

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
#include "sujfeyn.hpp"
#include "sujsoft.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#include <boost/version.hpp>
#include <boost/random/mersenne_twister.hpp>

#if (BOOST_VERSION >= 104700)
  #include <boost/random/uniform_int_distribution.hpp>
  typedef boost::random::uniform_int_distribution<> uniform_int_dist;
  typedef boost::random::mt19937 boost_mt19937;
#else
  #include <boost/random/uniform_int.hpp>
  typedef boost::uniform_int<> uniform_int_dist;
  typedef boost::mt19937 boost_mt19937;
#endif

#define NUM_REQ_ARGS 2
#define NUM_REQ_ARGS_ALT 4

using namespace std;

//extern string suj_slha_out(double m0, double mhf, double a0, double tb, double sgnMu, MssmSoftsusy r);
extern string suj_slha_out(double mGUT, const DoubleVector &pars, double tb, double sgnMu, MssmSoftsusy r);

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

	bool nusugra = true;



	const unsigned int tev = 1000;
	boost_mt19937 gen;
	uniform_int_dist m0_dist(10*10, 4*tev*10); // divide m0 by 1e1
	uniform_int_dist mhf_dist(10*10, 5*tev*10); // divide mhf by 1e1
	uniform_int_dist a0_dist(-8*1000,8*1000); // divide a0 by 1e3
	uniform_int_dist tb_dist(1*1000,60*1000); //divide tb by 1e3


	uniform_int_dist m1_dist(10*10, 5*tev*10); // divide m1 by 1e1
	uniform_int_dist m2_dist(10*10, 5*tev*10); // divide m2 by 1e1
	uniform_int_dist m3_dist(10*10, 5*tev*10); // divide m3 by 1e1

	cerr << "Setting seed to " << seed << " ..." << endl;
	gen.seed(seed);


	/***** SOFTSUSY Preamble *****/

	double m0, mhf, a0, tb; 
	double m1, m2, m3; // NU-G only

	cerr << "Searching for mSUGRA parameter points..." << endl;
	int npoints = 0;

	model_parser mp;
	micromegas_driver micro;
	while( npoints < 1000000 ) // arbitrary stopping condition
	{

		DoubleVector pars(3);

		if (!nusugra)
		{
			pars.setEnd(3); // mSUGRA pars: m0, mhf, a0

			m0 = m0_dist(gen)/1e1;
			mhf = mhf_dist(gen)/1e1;
			a0 = m0*a0_dist(gen)/1e3;

			pars(1) = m0;
			pars(2) = mhf;
			pars(3) = a0;

		}
		else  // just do NU-G right now
		{
			pars.setEnd(49); // NUSUGRA pars: not actually 49, but that's the index of the last one

			m0 = m0_dist(gen)/1e1;
			m1 = m1_dist(gen)/1e1;
			m2 = m2_dist(gen)/1e1;
			m3 = m3_dist(gen)/1e1;
			a0 = m0*a0_dist(gen)/1e3;
			
			// gaugino masses
			pars(1) = m1;
			pars(2) = m2;
			pars(3) = m3;

			// At, Ab, Atau
			pars(11) = a0;
			pars(12) = a0;
			pars(13) = a0;

			// Higgs field squared-masses
			pars(21) = m0*m0;
			pars(22) = m0*m0;

			// scalar leptons
			for (int slepton = 31; slepton <= 36; slepton++)
				pars(slepton) = m0;

			// scalar quarks
			for (int squark = 41; squark <= 49; squark++)
				pars(squark) = m0;
		}

		tb = tb_dist(gen)/1e3;

		softsusy_opts point; // default choices for SM nuisance params
		point.set_pars(pars);
		point.set_tb(tb);
		// keep default mGUT guess and sgn(mu)

		model m;

		softsusy_driver softsusy(&point); // only does QED/QCD running of Q/l masses
		feynhiggs_driver feynhiggs;
		micromegas_driver micro;

		try { 
			m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 
		} catch (const string &s) { cerr << "SOFTSUSY exception: " << s << endl; continue;}

		feynhiggs(&m);
		micro(&m);

		// everything should be good here...
		npoints++;

		writer << m << endl;
	}

	if (writer.is_open()) writer.close();

	return 0;
}


