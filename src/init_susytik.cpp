

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

using namespace std;

extern string suj_slha_out(double m0, double mhf, double a0, double tb, double sgnMu, MssmSoftsusy r);

int main(int argc, char** argv)
{
	/// Sets up exception handling
	signal(SIGFPE, FPE_ExceptionHandler);

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

	int npoints = 0;

	model_parser mp;
	micromegas_driver micro(micromegas_driver::init);

	m0 = 4000;
	mhf = 200;
	a0 = 4000;
	tb = 45;

	DoubleVector pars(3);
	pars(1) = m0;
	pars(2) = mhf;
	pars(3) = a0;

	MssmSoftsusy r;

	r.lowOrg(sugraBcs, mGutGuess, pars, sgnMu, tb, oneset, true);

	if (r.displayProblem().test())
	{
		cerr << "Problem with test point! Please change it!" << endl;
		return 1;
	}
	
	model sdb = mp.parse(suj_slha_out(m0, mhf, a0, tb, sgnMu, r));
	if (model::invalid == sdb.get_model_type())
	{
		cerr << "Problem parsing point data! Please use a different point!" << endl;
		return 2;
	}

	if (susy_dict::m_o1 != sdb.get_hierarchy(0))
	{
		cerr << "Neutralino-1 is not the LSP! Please use a different point!" << endl;
		return 3;
	}

	model mdb = micro(sdb);

	if (model::invalid == mdb.get_model_type() || model::obs_invalid == mdb.get_observable_data_type())
	{
		cerr << "Problem with obervables for test point! Please change it!" << endl;
		return 4;
	}

	cout << "\\Omega_\\chi h^2 = " << mdb.get_observable(susy_dict::observable::omega) << endl;

	return 0;
}


