
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

#include "libconstrain.h"
#include "sujmicro.h"

#include "mycomplex.h"
#include "def.h"
#include "linalg.h"
#include "lowe.h"
#include "rge.h"
#include "softsusy.h"
#include "softpars.h"
#include "susy.h"
#include "utils.h"
#include "numerics.h"
#include "physpars.h"

#include "nestface.h"
#include "hepstats.hpp"

#include <boost/math/special_functions/pow.hpp>

#define NUM_REQ_ARGS 4

#define NUSUGRA

const double m0_min = 1.7;
const double m0_max = 3.904;
const double mhf_min = 1.7;
const double mhf_max = 3.7;
const double a0_min = -30e3;
const double a0_max = 30e3;
const double tb_min = 3;
const double tb_max = 60;
const int sgnmu = 1;

#ifdef NUSUGRA
// non-universal
const int extra_params = 0; // for SU(5) or SO(10) IR's no extras needed
#else
const int extra_params = 0;
#endif

const double mtop_min = 173.5 - 2*1.0;
const double mtop_max = 173.5 + 2*1.0;
const double alpha_s_min = 0.1187 - 2*7e-4;
const double alpha_s_max = 0.1187 + 2*7e-4;
const double alpha_eminv_min = 127.933 - 2*1.4e-2;
const double alpha_eminv_max = 127.933 + 2*1.4e-2;
const double mb_min = 4.18 - 2*3e-2;
const double mb_max = 4.18 + 2*3e-2;

const double logZero = -1e90;

static std::string lockfile;

void exit_without_unlocking()
{
	std::cerr << "Exiting too early to unlock!" << std::endl;
	return;
}

void exit_with_unlocking()
{
	if (lockfile.size() > 0)
	{
		std::cerr << "Deleting LOCK file: " << lockfile << "..." << std::endl;
		unlink(lockfile.c_str());
	}
	else
	{
		std::cerr << "LOCK file variable not set!" << std::endl;
	}
	return;
}

namespace nestface_common
{
	hepstats::loglike llhood;
}


void log_like_request(double *Cube, int &ndim, int &npars, double &lnew, void *context);

extern string suj_slha_out(double mGUT, const DoubleVector &pars, double tb, double sgnMu, MssmSoftsusy r);
double get_mapped(double normed, double min, double max, bool log);
void get_pars(double *Cube, const int &npars);

void dumper(int &nSamples, int &nlive, int &nPar, double **physLive, double **posterior, double **paramConstr, double &maxLogLike, double &logZ, double &logZerr, void *context);

int main(int argc, char**argv)
{
	atexit(exit_without_unlocking);
	if (NUM_REQ_ARGS != argc)
	{
		std::cerr << "Usage: " << argv[0] << "<output directory> <output filename root> <likelihood config file>" << std::endl
			  << std::endl;
		return 1;
	}

	std::string out_dir, out_root, filename;

	out_dir = std::string(argv[1]);
	out_root = std::string(argv[2]);
	filename = std::string(argv[3]);

	if (0 != mkdir(out_dir.c_str(),S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
	{
		if (EEXIST == errno)
		{
			std::cerr << out_dir << " exists... continuing..." << std::endl;
		}
		else
		{
			std::cerr << "Error creating " << out_dir << ": " << strerror(errno) << std::endl;
			return 2;
		}
	}

	std::stringstream ss;
	ss << out_dir << "/" << out_root;
	if (ss.str().size() > 100)
	{
		std::cerr << "Error: director+root larger than arbitrary cap. Feel free to change this." << std::endl;
		return 3;
	}

	std::cerr << "Reading likelihood function configuration from " << filename << "..." << endl;
	std::ifstream reader;
	reader.open(filename.c_str());
	if (reader.fail())
	{
		cerr << "Error opening " << filename << " for reading!" << endl;
		return 1;
	}

	hepstats::likeconfig config(&reader);
	nestface_common::llhood = config();

	if (reader.is_open()) 
		reader.close();



	int mmodal = 1;					// do mode separation?
	int ceff = 0;					// run in constant efficiency mode?
	int nlive = 5000;				// number of live points
	
	double efr = 0.8;				// set the required efficiency
	
	double tol = 0.01;				// tol, defines the stopping criteria
	
	int ndims = 8+extra_params;					// dimensionality (no. of free parameters)
#ifdef NUSUGRA	
	int nPar = ndims + (susy_dict::NUSUGRA_row.size()-1) + susy_dict::observable::observe_row.size();	// total no. of parameters including free & derived parameters
#else
	int nPar = ndims + (susy_dict::mSUGRA_row.size()-1) + susy_dict::observable::observe_row.size();	// total no. of parameters including free & derived parameters
#endif
	
	int nClsPar = 4;				// no. of parameters to do mode separation on
	
	int updInt = 200;				// after how many iterations feedback is required & the output files should be updated
							// note: posterior files are updated & dumper routine is called after every updInt*10 iterations
	
	double Ztol = logZero;				// all the modes with logZ < Ztol are ignored
	
	int maxModes = 100;				// expected max no. of modes (used only for memory allocation)
	
	int pWrap[ndims];				// which parameters to have periodic boundary conditions?
	for(int i = 0; i < ndims; i++) pWrap[i] = 0;
	
	char root[100];					// root for output files
	strcpy(root, ss.str().c_str());
	
	int seed = -1;					// random no. generator seed, if < 0 then take the seed from system clock
	
	int fb = 1;					// need feedback on standard output?
	
	int resume = 1;					// resume from a previous job?
	
	int outfile = 1;				// write output files?
	
	int initMPI = 1;				// initialize MPI routines?, relevant only if compiling with MPI
							// set it to F if you want your main program to handle MPI initialization
	
	double logzero = logZero;				// points with loglike < logZero will be ignored by MultiNest
	
	int maxiter = 0;				// max no. of iterations, a non-positive value means infinity. MultiNest will terminate if either it 
							// has done max no. of iterations or convergence criterion (defined through tol) has been satisfied
	
	void *context = 0;				// not required by MultiNest, any additional information user wants to pass

#if 0
	struct stat stat_p;
	lockfile = std::string(ss.str()+".LOCK");
	if (!stat(lockfile.c_str(),&stat_p) && S_ISREG(stat_p.st_mode))
	{
		do
		{
			std::cerr << "LOCK file exists: " << lockfile << ". Sleeping..." << std::endl;
			sleep(30);
		} while (!stat(lockfile.c_str(),&stat_p) && S_ISREG(stat_p.st_mode));
	}
	std::fstream f(lockfile.c_str(), std::fstream::out | std::fstream::trunc);
	f << "LOCKED!" << std::endl;
	atexit(exit_with_unlocking);
#endif	
	// calling MultiNest

	nested::run(mmodal, ceff, nlive, tol, efr, ndims, nPar, nClsPar, maxModes, updInt, Ztol, root, seed, pWrap, fb, resume, outfile, initMPI,
	logzero, maxiter, log_like_request, dumper, context);

	return 0;
}

double get_mapped(double normed, double min, double max, bool log)
{
	double val = normed*(max-min) + min;
	if (log)
		return std::pow(10.0, val);
	return val;
}

void get_pars(double *Cube, const int &npars)
{
	if (npars < 8+extra_params)
		return;
	
	double m0 = get_mapped(Cube[0], m0_min, m0_max, true);
	double mhf = get_mapped(Cube[1], mhf_min, mhf_max, true);
	double a0 = get_mapped(Cube[2], a0_min, a0_max, false);
	double tb = get_mapped(Cube[3], tb_min, tb_max, false);

	Cube[0] = m0;
	Cube[1] = mhf;
	Cube[2] = a0;
	Cube[3] = tb;

	// nuisance
	double mtop = get_mapped(Cube[4], mtop_min, mtop_max, false);
	double alpha_s = get_mapped(Cube[5], alpha_s_min, alpha_s_max, false);
	double alpha_eminv = get_mapped(Cube[6], alpha_eminv_min, alpha_eminv_max, false);
	double mb = get_mapped(Cube[7], mb_min, mb_max, false);

	Cube[4] = mtop;
	Cube[5] = alpha_s;
	Cube[6] = alpha_eminv;
	Cube[7] = mb;

#ifdef NUSUGRA
	// non-universal

	// nothing to do this time...
#endif

	return;
}

/************************************************* dumper routine ******************************************************/

// The dumper routine will be called every updInt*10 iterations
// MultiNest doesn not need to the user to do anything. User can use the arguments in whichever way he/she wants
//
//
// Arguments:
//
// nSamples 						= total number of samples in posterior distribution
// nlive 						= total number of live points
// nPar 						= total number of parameters (free + derived)
// physLive[1][nlive * (nPar + 1)] 			= 2D array containing the last set of live points (physical parameters plus derived parameters) along with their loglikelihood values
// posterior[1][nSamples * (nPar + 2)] 			= posterior distribution containing nSamples points. Each sample has nPar parameters (physical + derived) along with the their loglike value & posterior probability
// paramConstr[1][4*nPar]:
// paramConstr[0][0] to paramConstr[0][nPar - 1] 	= mean values of the parameters
// paramConstr[0][nPar] to paramConstr[0][2*nPar - 1] 	= standard deviation of the parameters
// paramConstr[0][nPar*2] to paramConstr[0][3*nPar - 1] = best-fit (maxlike) parameters
// paramConstr[0][nPar*4] to paramConstr[0][4*nPar - 1] = MAP (maximum-a-posteriori) parameters
// maxLogLike						= maximum loglikelihood value
// logZ							= log evidence value
// logZerr						= error on log evidence value
// context						void pointer, any additional information


void dumper(int &nSamples, int &nlive, int &nPar, double **physLive, double **posterior, double **paramConstr, double &maxLogLike, double &logZ, double &logZerr, void *context)
{
	return;
}

/******************************************** loglikelihood routine ****************************************************/

// Input arguments
// ndim 						= dimensionality (total number of free parameters) of the problem
// npars 						= total number of free plus derived parameters
// context						void pointer, any additional information
//
// Input/Output arguments
// Cube[npars] 						= on entry has the ndim parameters in unit-hypercube
//	 						on exit, the physical parameters plus copy any derived parameters you want to store with the free parameters
//	 
// Output arguments
// lnew 						= loglikelihood

void log_like_request(double *Cube, int &ndim, int &npars, double &lnew, void *context)
{
	get_pars(Cube, npars);


	double m0 = Cube[0], mhf = Cube[1], a0 = Cube[2], tb = Cube[3], m_top = Cube[4], alpha_s = Cube[5], alpha_em_inv = Cube[6], mb = Cube[7];
#ifdef NUSUGRA
	double delta_h1 = Cube[8], delta_h2 = Cube[9];
#endif

#ifndef NUSUGRA
	std::cerr << "Now trying " << m0 << " " << mhf << " " << a0 << " " << tb << " 2e16 1 " << mb << " " << m_top << " " << alpha_s << " " << alpha_em_inv << " ..."; // don't flush!
#else
	std::cerr << "Now trying " << m0 << " " << mhf << " " << a0 << " " << tb << " 2e16 1 " << mb << " " << m_top << " " << alpha_s << " " << alpha_em_inv << " ..."; // don't flush!
#endif

	DoubleVector pars(3);
#ifndef NUSUGRA

	pars(1) = Cube[0]; // m0
	pars(2) = Cube[1]; // mhf
	pars(3) = Cube[2]; // a0

#else

	pars.setEnd(49); // NUSUGRA pars: not actually 49, but that's the index of the last one

	// gaugino masses
	pars(1) = mhf*10.0; //m1
	pars(2) = mhf*2.0; //m2
	pars(3) = mhf; //m3

	// At, Ab, Atau
	pars(11) = a0; //at
	pars(12) = a0; //ab
	pars(13) = a0; //atau

	// Higgs field squared-masses
	pars(21) = boost::math::pow<2>(m0); //mh1^2
	pars(22) = boost::math::pow<2>(m0); //mh2^2

	pars(31) = m0; //meL
	pars(32) = m0; //mmuL
	pars(33) = m0; //mtauL
	pars(34) = m0; //meR
	pars(35) = m0; //mmuR
	pars(36) = m0; //mtauR

	pars(41) = m0; //mqL1
	pars(42) = m0; //mqL2
	pars(43) = m0; //mqL3
	pars(44) = m0; //muR
	pars(45) = m0; //mcR
	pars(46) = m0; //mtR
	pars(47) = m0; //mdR
	pars(48) = m0; //msR
	pars(49) = m0; //mbR
#endif

	QedQcd oneset;
	oneset.setAlpha(ALPHAS, alpha_s); // alpha_s(MZ)
	oneset.setAlpha(ALPHA, 1.0/alpha_em_inv); // 1/alpha_em(MZ)

	oneset.setPoleMt(m_top);
	oneset.setMass(mBottom, mb); 
	oneset.toMz();

	micromegas_driver micro;
	MssmSoftsusy r;
	model_parser mp;

#ifndef NUSUGRA
	double mGUT = r.lowOrg(sugraBcs, 2.0e16, pars, sgnmu, tb, oneset, true);
#else
	double mGUT = r.lowOrg(extendedSugraBcs, 2.0e16, pars, sgnmu, tb, oneset, true);
#endif

	if (r.displayProblem().testSeriousProblem())
	{
		std::cerr << " SoftSUSY Error: " << r.displayProblem() << std::endl;
		lnew = logZero;
		return;
	}

	model sdb = mp.parse(suj_slha_out(mGUT, pars, tb, sgnmu, r), false); //false because it's not merged yet

	if (model::invalid == sdb.get_model_type())
	{
		lnew = logZero;
		return;
	}

	if (susy_dict::m_o1 != sdb.get_hierarchy(0))
	{
		std::cerr << "wrong LSP!" << std::endl;
		lnew = logZero;
		return;	  // neutralino1 is not the LSP...
			  // may use a tolerance in the 
			  // mass gap in the future
	}

	model mdb = micro(sdb);
	if (model::invalid == mdb.get_model_type() || model::obs_invalid == mdb.get_observable_data_type())
	{
		lnew = logZero;
		std::cerr << "micrOMEGAs error!" << std::endl;
		return; //problem with observables (micromegas)
	}

	lnew = nestface_common::llhood.get_log_like(mdb);


#ifndef NUSUGRA
	auto row_begin = susy_dict::mSUGRA_row.begin();
	auto row_end= susy_dict::mSUGRA_row.end();
	unsigned int sugra_length = susy_dict::mSUGRA_row.size()-1; // skipping model name
#else
	auto row_begin = susy_dict::NUSUGRA_row.begin();
	auto row_end= susy_dict::NUSUGRA_row.end();
	unsigned int sugra_length = susy_dict::NUSUGRA_row.size()-1; // skipping model name
#endif

	if (npars != ndim + sugra_length + susy_dict::observable::observe_row.size())
	{
		lnew = logZero;
		std::cerr << "Cube[npars] has the wrong length!!" << endl;
		return;
	}

	std::transform(
		++row_begin, // skip the model name
		row_end,
		Cube+ndim, 			 // these start after free params
		[&mdb] (const std::string &key) -> double {
			return mdb.get_datum(key);
		}
	);

	Cube[ndim+sugra_length] = 1.0; // valid bit from observables

	row_begin = susy_dict::observable::observe_row.begin();
	row_end = susy_dict::observable::observe_row.end();

	std::transform(
		++row_begin,
		row_end,
		Cube+ndim+sugra_length+1, // these start after SLHA data -- +1 is for valid_bit
		[&mdb] (const std::string &key) -> double {
			return mdb.get_observable(key);
		}
	);


	std::cerr << " :: m_h = " << mdb.get_datum(susy_dict::m_h0) << ", ln(like) = " << lnew << std::endl;
}



