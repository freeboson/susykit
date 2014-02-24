

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

#if 0
bool nusugra = false;
#define NUM_REQ_ARGS 11
#else
bool nusugra = true;
#define NUM_REQ_ARGS 13
#endif

using namespace std;

extern string suj_slha_out(double mGUT, const DoubleVector &pars, double tb, double sgnMu, MssmSoftsusy r);

void usage(const string &s)
{

//	ss >> m0 >> mhf >> a0 >> tb >> mGUT >> sgnMu >> mb >> mt >> alphas >> alpha_em_inv;
	cerr 	<< "Usage: " << s << " <m0> <mhf> <a0> <tb> <mGUT> <sgn(mu)> <mb(mb)> <mtop pole> <alpha_S> <1/alpha_{EM}>" << endl
		<< endl
		<< "Note, according to PDG2012," << endl
		<< endl
		<< "mb(mb) = (4.18 +/- 0.03) GeV" << endl
		<< "mtop (pole) = (173.5 +/0 1.0) GeV" << endl
		<< "alpha_S(M_Z) = 0.1184 +/- 0.0007" << endl
		<< "1/alpha_{EM}(M_Z) = 127.944 +/- 0.014" << endl
		<< endl
		<< "so for the last few params you could use: 2e16 1 4.18 173.5 0.1184 127.944" << endl
		<< endl;
	return;
}

int main(int argc, char** argv)
{
	/// Sets up exception handling
	signal(SIGFPE, FPE_ExceptionHandler);

	unsigned int seed;

	if (NUM_REQ_ARGS != argc)
	{
		usage(argv[0]);
		return 1;
	}

	seed = time(NULL);

	stringstream ss;
	for (int i = 1; i < NUM_REQ_ARGS; i++)
		ss << argv[i] << " ";
	double m0, mhf, a0, tb, mGutGuess;
	int sgnMu;
	double mb, mt, alphas, alpha_em_inv;

	double m1, m2, m3;

	if (!nusugra)
		ss >> m0 >> mhf >> a0 >> tb >> mGutGuess >> sgnMu >> mb >> mt >> alphas >> alpha_em_inv;
	else
		ss >> m0 >> m1 >> m2 >> m3 >> a0 >> tb >> mGutGuess >> sgnMu >> mb >> mt >> alphas >> alpha_em_inv;
		
	cerr << "Setting SOFTSUSY params and running SM fermion masses..." << endl;
	/***** SOFTSUSY Preamble *****/

	QedQcd oneset;      ///< See "lowe.h" for default definitions parameters

	/// most important Standard Model inputs: you may change these and recompile
	oneset.setAlpha(ALPHA, 1/alpha_em_inv);
	oneset.setAlpha(ALPHAS, alphas);
	oneset.setPoleMt(mt);
	oneset.setMass(mBottom, mb);

	oneset.toMz();      ///< Runs SM fermion masses to MZ

	model_parser mp;
	micromegas_driver micro;

	DoubleVector pars(3);

	pars.setEnd(3); // mSUGRA pars: m0, mhf, a0

	if (!nusugra)
	{
		pars.setEnd(3); // mSUGRA pars: m0, mhf, a0

		pars(1) = m0;
		pars(2) = mhf;
		pars(3) = a0;

	}
	else  // just do NU-G right now
	{
		pars.setEnd(49); // NUSUGRA pars: not actually 49, but that's the index of the last one

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


	MssmSoftsusy r;
	double mGUT;

	if (!nusugra)
		mGUT = r.lowOrg(sugraBcs, mGutGuess, pars, sgnMu, tb, oneset, true);
	else
		mGUT = r.lowOrg(extendedSugraBcs, mGutGuess, pars, sgnMu, tb, oneset, true);

	if (r.displayProblem().test())
	{
		cerr << "SOFTSUSY Problem: " << r.displayProblem() << endl;
		return 1;
	}

	model sdb = mp.parse(suj_slha_out(mGUT, pars, tb, sgnMu, r), false); //false because it's not merged yet

	if (model::invalid == sdb.get_model_type())
	{
		cerr << "Parsing Error!" << endl;
		return 2;
	}

	if (susy_dict::m_o1 != sdb.get_hierarchy(0))
	{
		cerr << "Wrong LSP (" << sdb.get_hierarchy(0) << "=" << sdb.get_datum(sdb.get_hierarchy(0)) << " GeV; " << susy_dict::m_o1 << "=" << sdb.get_datum(susy_dict::m_o1) << "GeV)!" << endl;
//		return 3;
	}

	model mdb = micro(sdb);

	if (model::invalid == mdb.get_model_type() || model::obs_invalid == mdb.get_observable_data_type())
	{
		cerr << "MicrOMEGAs Error!" << endl;
		return 4;
	}

	cout << mdb << endl;

	return 0;
}


