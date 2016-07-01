/*

   **************************************************************
   *                                                            *
   *                                                            *
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

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/parse.hpp"
#include "constrain/get_slha.hpp"
#include "constrain/special_lookups.hpp"

// softsusy
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

// feynhiggs
#include "susykit/feynhiggs_interface.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#define NUM_REQ_ARGS 4
#define NUM_REQ_ARGS_ALT 4

using namespace std;

extern string suj_slha_out(double mGUT, const DoubleVector &pars, double tb, double sgnMu, MssmSoftsusy r);

int main(int argc, char **argv) {
    /// Sets up exception handling
    signal(SIGFPE, FPE_ExceptionHandler);

    unsigned int seed;

    if (1 + NUM_REQ_ARGS != argc && 1 + NUM_REQ_ARGS_ALT != argc) {
//		usage(argv[0]);
        return 1;
    }

    bool nusugra = false;


    cerr << "Setting SOFTSUSY params and running SM fermion masses..." << endl;

    /***** SOFTSUSY Preamble *****/

    /// Parameters used: mSUGRA parameters
    double mGutGuess = 2.0e16;
    int sgnMu = 1;      ///< sign of mu parameter

    QedQcd oneset;      ///< See "lowe.h" for default definitions parameters

    /// most important Standard Model inputs: you may change these and recompile
    double alpha_em_inv = 127.933, alphasMZ = 0.1187, mtop = 173.1, mbmb = 4.18;
    oneset.setAlpha(ALPHA, 1.0 / alpha_em_inv);
    oneset.setAlpha(ALPHAS, alphasMZ);
    oneset.setPoleMt(mtop);
    oneset.setMass(mBottom, mbmb);

    oneset.toMz();      ///< Runs SM fermion masses to MZ

    /***** SOFTSUSY Preamble *****/

    double m0, mhf, a0, tb;
    double m1, m2, m3; // NU-G only
    double mGUT;

    for (int i = 1; i <= 4; i++) {
        stringstream ss(argv[i]);
        switch (i) {
            case 1:
                ss >> m0;
                break;
            case 2:
                ss >> mhf;
                break;
            case 3:
                ss >> a0;
                break;
            case 4:
                ss >> tb;
                break;
            default:
                return 2;
        }
    }

    cerr << "m0 = " << m0 << " mhf = " << mhf << " a0 = " << a0 << " tb = " << tb << endl;

    model_parser mp;
    get_slha slha;
    feynhiggs_driver feynhiggs;

    DoubleVector pars(3);

    if (!nusugra) {
        pars.setEnd(3); // mSUGRA pars: m0, mhf, a0

        pars(1) = m0;
        pars(2) = mhf;
        pars(3) = a0;

    }

    MssmSoftsusy r;

    if (!nusugra)
        r.lowOrg(sugraBcs, mGutGuess, pars, sgnMu, tb, oneset, true);

    r.displayMxBC();

    if (r.displayProblem().test()) {
        cerr << "SOFTSUSY Error: " << r.displayProblem() << endl;
        return 3; //problem with RGE
    }

    model sdb = mp.parse(suj_slha_out(mGUT, pars, tb, sgnMu, r), false); //false because it's not merged yet

    if (model::invalid == sdb.get_model_type()) {
        cerr << "SLHA Data Parse Error!" << endl;
        return 4; //problem with parsing (unusual)
    }

    if (susy_dict::m_o1 != sdb.get_hierarchy(0)) {
        cerr << "Non-neutralino LSP!" << endl;
        return 5; // neutralino1 is not the LSP...
        // may use a tolerance in the
        // mass gap in the future
    }

    cout << slha(sdb) << endl;
    feynhiggs(&sdb);

    return 0;
}


