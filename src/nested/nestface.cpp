
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
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

#define NUM_REQ_ARGS 4

const double m0_min = 1.7;
const double m0_max = 3.904;
const double mhf_min = 1.7;
const double mhf_max = 3.7;
const double a0_min = -30e3;
const double a0_max = 30e3;
const double tb_min = 3;
const double tb_max = 60;
const int sgnmu = 1;

const double mtop_min = 173.5 - 2*1.0;
const double mtop_max = 173.5 + 2*1.0;
const double alpha_s_min = 0.1187 - 2*7e-4;
const double alpha_s_max = 0.1187 + 2*7e-4;
const double alpha_eminv_min = 127.933 - 2*1.4e-2;
const double alpha_eminv_max = 127.933 + 2*1.4e-2;
const double mb_min = 4.18 - 2*3e-2;
const double mb_max = 4.18 + 2*3e-2;

const double logZero = -1e90;

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
	
	double tol = 0.1;				// tol, defines the stopping criteria
	
	int ndims = 8;					// dimensionality (no. of free parameters)
	
	int nPar = ndims + (susy_dict::mSUGRA_row.size()-1) + susy_dict::observable::observe_row.size();	// total no. of parameters including free & derived parameters
	
	int nClsPar = 4;				// no. of parameters to do mode separation on
	
	int updInt = 200;				// after how many iterations feedback is required & the output files should be updated
							// note: posterior files are updated & dumper routine is called after every updInt*10 iterations
	
	double Ztol = logZero;				// all the modes with logZ < Ztol are ignored
	
	int maxModes = 100;				// expected max no. of modes (used only for memory allocation)
	
	int pWrap[ndims];				// which parameters to have periodic boundary conditions?
	for(int i = 0; i < ndims; i++) pWrap[i] = 0;
	
//	char root[100] = "realtest/realtest";			// root for output files
	char root[100];
	strcpy(root, ss.str().c_str());
	
	int seed = -1;					// random no. generator seed, if < 0 then take the seed from system clock
	
	int fb = 1;					// need feedback on standard output?
	
	int resume = 1;					// resume from a previous job?
	
	int outfile = 1;				// write output files?
	
	int initMPI = 0;				// initialize MPI routines?, relevant only if compiling with MPI
							// set it to F if you want your main program to handle MPI initialization
	
	double logzero = logZero;				// points with loglike < logZero will be ignored by MultiNest
	
	int maxiter = 0;				// max no. of iterations, a non-positive value means infinity. MultiNest will terminate if either it 
							// has done max no. of iterations or convergence criterion (defined through tol) has been satisfied
	
	void *context = 0;				// not required by MultiNest, any additional information user wants to pass

	
	
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
	if (npars < 8)
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

	DoubleVector pars(3);

	std::cerr << "Now trying " << Cube[0] << " " << Cube[1] << " " << Cube[2] << " " << Cube[3] << " 2e16 1 " << Cube[7] << " " << Cube[4] << " " << Cube[5] << " " << Cube[6] << " ..." << std::flush;

	pars(1) = Cube[0]; // m0
	pars(2) = Cube[1]; // mhf
	pars(3) = Cube[2]; // a0

	QedQcd oneset;
	oneset.setAlpha(ALPHAS, Cube[5]); // alpha_s(MZ)
	oneset.setAlpha(ALPHA, 1.0/Cube[6]); // 1/alpha_em(MZ)

	oneset.setPoleMt(Cube[4]);
	oneset.setMass(mBottom, Cube[7]);

	oneset.toMz();

	micromegas_driver micro;
	MssmSoftsusy r;
	model_parser mp;

	double mGUT = r.lowOrg(sugraBcs, 2.0e16, pars, sgnmu, Cube[3], oneset, true);

	if (r.displayProblem().testSeriousProblem())
	{
		std::cerr << " SoftSUSY Error: " << r.displayProblem() << std::endl;
		lnew = logZero;
		return;
	}

	model sdb = mp.parse(suj_slha_out(mGUT, pars, Cube[3], sgnmu, r), false); //false because it's not merged yet

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

	if (npars != ndim + (susy_dict::mSUGRA_row.size()-1) + susy_dict::observable::observe_row.size())
	{
		std::cerr << "Cube[npars] has the wrong length!!" << endl;
		return;
	}

	auto row_begin = susy_dict::mSUGRA_row.begin();

	std::transform(
		++row_begin, // skip the model name
		susy_dict::mSUGRA_row.end(),
		Cube+ndim, 			 // these start after free params
		[&mdb] (const std::string &key) -> double {
			return mdb.get_datum(key);
		}
	);

	std::transform(
		susy_dict::observable::observe_row.begin(),
		susy_dict::observable::observe_row.end(),
		Cube+ndim+(susy_dict::mSUGRA_row.size()-1), // these start after mSUGRA SLHA data
		[&mdb] (const std::string &key) -> double {
			return mdb.get_observable(key);
		}
	);


	std::cerr << "m_h = " << mdb.get_datum(susy_dict::m_h0) << ", ln(like) = " << lnew << std::endl;
}



