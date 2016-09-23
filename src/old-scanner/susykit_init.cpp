/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - susykit_init.cpp: This is a defunct component.                           -
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

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/parse.hpp"
#include "constrain/get_slha.hpp"
#include "constrain/special_lookups.hpp"

#include "susykit/softsusy_interface.hpp"
#include "susykit/micromegas_interface.hpp"
#include "susykit/feynhiggs_interface.hpp"

#include <string>

using namespace std;


int main(int argc, char **argv) {

    softsusy_opts point;

    const double m0 = 6000.0, mhf = 200.0, a0 = -12000.0, tb = 25.0;

    DoubleVector pars(3);

    pars(1) = m0;
    pars(2) = mhf;
    pars(3) = a0;

    point.set_pars(pars);
    point.set_tb(tb);


    model m;

    softsusy_driver softsusy(&point);
    micromegas_driver micro;
    feynhiggs_driver feynhiggs;

    try {
        m = softsusy(); // need to check for displayProblem().test() and neutralino LSP
    } catch (const string &s) {
        cerr << "SOFTSUSY exception: " << s << endl;
        return 1;
    }

    feynhiggs(&m);
    micro(&m);

    if (model::invalid == m.get_model_type() || model::obs_invalid == m.get_observable_data_type()) {
        cerr << "Problem with obervables for test point! Please change it!" << endl;
//		return 4;
    }

    cout << "\\Omega_\\chi h^2 = " << m.get_observable(susy_dict::observable::omega) << endl;

    return 0;
}


