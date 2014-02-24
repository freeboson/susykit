

/*

   **************************************************************
   *                                                            *
   * SOFTSUSY Large Mu Scanner                                  *
   * Sujeet Akula                                               *
   * sujeet@freeboson.org                                       *
   *                                                            *
   * To compile:                                                *
   *                                                            *
   *                                                            *
   * To run:                                                    *
   *                                                            *
   *                                                            *
   **************************************************************

*/

#include <iostream>

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

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#define NUM_REQ_ARGS 2
#define NUM_REQ_ARGS_ALT 4

using namespace std;

extern string suj_slha_out(double m0, double mhf, double a0, double tb, double sgnMu, MssmSoftsusy r);

void usage(const string &s)
{
	cerr 	<< "Usage: " << s << " <output_file.mdb> [<job id / seed offset> <seed filename>]" << endl
		<< "Where, job id / seed offset is the line number in seed filename" << endl
		<< "that has the random seed for this run." << endl
		<< endl
		<< "output_file.mdb is required, since micrOMEGAs spews garbage to " << endl
		<< "stdout. Note that output_file.mdb will not be truncated; data " << endl
		<< "will be appended to the file." << endl
		<< endl;
	return;
}

int get_seed(const string &filename, unsigned int id, unsigned int *seed)
{
	fstream f(filename.c_str(), fstream::in);
	if (f.fail())
	{
		cerr << "Error opening " << filename << " for reading!" << endl;
		return 1;
	}

	unsigned int line_num;
	string line;
	for (line_num = 0; line_num < id && getline(f, line); line_num++);
	f.close();

	if (id == line_num)
	{
		stringstream ss(line);
		ss >> *seed;
		return 0;
	}
	else
	{
		cerr << "Hit EOF before reaching line number " << id << endl;
		return 2;
	}
}

int main(int argc, char** argv)
{
	/// Sets up exception handling
	signal(SIGFPE, FPE_ExceptionHandler);

	unsigned int seed;

	if (NUM_REQ_ARGS != argc && NUM_REQ_ARGS_ALT != argc)
	{
		usage(argv[0]);
		return 1;
	}

	string outfname = argv[1];
	fstream writer(outfname.c_str(), fstream::out | fstream::app);
	if (!writer.is_open() || writer.fail())
	{
		cerr << "Could not open " << outfname << " for writing!" << endl;
		return 2;
	}

	if (NUM_REQ_ARGS_ALT == argc)
	{
		cerr << "Looking up seed..." << endl;
		unsigned int id;
		string filename = argv[2];
		stringstream ss(argv[1]);
		ss >> id;
		if (get_seed(filename, id, &seed))
		{
			usage(argv[0]);
			return 3;
		}
	}
	else
	{
		cerr << "Setting seed by clock..." << endl;
		seed = time(NULL);
	}



	const unsigned int tev = 1000;
	boost::random::mt19937 gen;
	boost::random::uniform_int_distribution<> m0_dist(10*10, 4*tev*10); // divide m0 by 1e1
	boost::random::uniform_int_distribution<> mhf_dist(10*10, 5*tev*10); // divide mhf by 1e1
	boost::random::uniform_int_distribution<> a0_dist(-8*1000,8*1000); // divide a0 by 1e3
	boost::random::uniform_int_distribution<> tb_dist(1*1000,60*1000); //divide tb by 1e3

	cerr << "Setting seed to " << seed << " ..." << endl;
	gen.seed(seed);

	cerr << "Setting SOFTSUSY params and running SM fermion masses..." << endl;
	/***** SOFTSUSY Preamble *****/

	/// Parameters used: mSUGRA parameters
	double mGutGuess = 2.0e16;
	int sgnMu = 1;      ///< sign of mu parameter 

	QedQcd oneset;      ///< See "lowe.h" for default definitions parameters

	/// most important Standard Model inputs: you may change these and recompile
	double alphasMZ = 0.1184, mtop = 173.1, mbmb = 4.19;
	oneset.setAlpha(ALPHAS, alphasMZ);
	oneset.setPoleMt(mtop);
	oneset.setMass(mBottom, mbmb);

	oneset.toMz();      ///< Runs SM fermion masses to MZ

	/***** SOFTSUSY Preamble *****/

	double m0, mhf, a0, tb;

	cerr << "Searching for mSUGRA parameter points..." << endl;
	int npoints = 0;

	model_parser mp;
	micromegas_driver micro;
	while( npoints < 1000000 ) // arbitrary stopping condition
	{
		m0 = m0_dist(gen)/1e1;
		mhf = mhf_dist(gen)/1e1;
		a0 = m0*a0_dist(gen)/1e3;
		tb = tb_dist(gen)/1e3;

		DoubleVector pars(3);
		pars(1) = m0;
		pars(2) = mhf;
		pars(3) = a0;

		MssmSoftsusy r;

		r.lowOrg(sugraBcs, mGutGuess, pars, sgnMu, tb, oneset, true);

		if (r.displayProblem().test())
		{
			continue; //problem with RGE
		}

		model sdb = mp.parse(suj_slha_out(m0, mhf, a0, tb, sgnMu, r));

		if (model::invalid == sdb.get_model_type())
		{
			continue; //problem with parsing (unusual)
		}

		if (susy_dict::m_o1 != sdb.get_hierarchy(0))
		{
			continue; // neutralino1 is not the LSP...
				  // may use a tolerance in the 
				  // mass gap in the future
		}

		model mdb = micro(sdb);

		if (model::invalid == mdb.get_model_type() || model::obs_invalid == mdb.get_observable_data_type())
		{
			continue; //problem with observables (micromegas)
		}

		// everything should be good here...
		npoints++;

		writer << mdb << endl;
	}

	if (writer.is_open()) writer.close();

	return 0;
}


