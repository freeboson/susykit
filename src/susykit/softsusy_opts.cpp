/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - softsusy_opts.cpp:                                                       -
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


#include "susykit/softsusy_opts.hpp"

softsusy_opts::softsusy_opts()
        : pars(0), tb(default_tb), sgnmu(default_sgnmu), mgut_guess(default_mgut), alpha_s(default_alpha_s),
          alpha_em_inv(default_alpha_em_inv), mtop_pole(default_mtop), mbmb(default_mbmb) {

    // leaves uninitialized: tb, sgnmu, nusugra

}


SoftSusyBCFnPtr softsusy_opts::get_bcs() const {
    if (nusugra)
        return extendedSugraBcs;
    else
        return sugraBcs;

}


