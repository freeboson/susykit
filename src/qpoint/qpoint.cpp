/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - qpoint.cpp:                                                              -
    -                                                                          -
    - This file is part of SusyKit, https://freeboson.org/susykit/.            -
    -                                                                          -
    - SusyKit is free software: you can redistribute it and/or modify it under -
    - the terms of the GNU Lesser General Public License as published by the   -
    - Free Software Foundation, either version 3 of the License, or (at your   -
    - option) any later version.                                               -
    -                                                                          -
    - SusyKit is distributed in the hope that it will be useful, but WITHOUT   -
    - ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    -
    - FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     -
    - License for more details.                                                -
    -                                                                          -
    - You should have received a copy of the GNU Lesser General Public License -
    - along with SusyKit.  If not, see <http://www.gnu.org/licenses/>.         -
    -                                                                          -
    ----------------------------------------------------------------------------

*/


#include <iostream>

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/parse.hpp"
#include "constrain/get_slha.hpp"
#include "constrain/special_lookups.hpp"

//#include "susykit/micromegas_interface.hpp"
#include "susykit/softsusy_interface.hpp"
#include "susykit/feynhiggs_interface.hpp"
#include "susykit/darksusy_interface.hpp"
#include "susykit/superiso_interface.hpp"

#include "qpoint_softsusy_opts.hpp"

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

    get_slha slha;

    softsusy_driver softsusy(sugra);
//    micromegas_driver micro;
    feynhiggs_driver feynhiggs;
	darksusy_driver darksusy;
    superiso_driver superiso;

    try {
        m = softsusy(); // need to check for displayProblem().test() and neutralino LSP
    } catch (const string &s) {
        cerr << "SOFTSUSY exception: " << s << endl;
        return 1;
    }


    feynhiggs(&m);
//    micro(&m);
	darksusy(&m);
	superiso(&m);

    cout << m << endl;

    if (sugra->slha())
        cerr << slha(m) << endl;

    delete sugra;

    return 0;
}


