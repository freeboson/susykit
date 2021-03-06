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

#include "susykit/micromegas_interface.hpp"
#include "susykit/softsusy_interface.hpp"
#include "susykit/feynhiggs_interface.hpp"
#include "susykit/superiso_interface.hpp"

#include "../qpoint/qpoint_softsusy_opts.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    /// Sets up exception handling
    signal(SIGFPE, FPE_ExceptionHandler);

    qpoint_opts *sugra;
    try {
        sugra = new qpoint_opts(argc, argv);
    } catch (exception &e) {
        if (sugra != nullptr)
            delete sugra;
        return 1;
    }

    model m;

    softsusy_driver softsusy(sugra);
    micromegas_driver micro;
    feynhiggs_driver feynhiggs;

    try {
        m = softsusy(); // need to check for displayProblem().test() and neutralino LSP
    } catch (const string &s) {
        cerr << "SOFTSUSY exception: " << s << endl;
        return 1;
    }


    micro(&m);
    feynhiggs(&m);

    cout << m << endl;

    delete sugra;

    return 0;
}


