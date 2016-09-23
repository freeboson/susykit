/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - softsusy_opts.hpp:                                                       -
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


#pragma once
#ifndef SOFTSUSY_OPTS_HPP
#define SOFTSUSY_OPTS_HPP

#include "softsusy/softsusy.h"
#include "softsusy/linalg.h"

#include <stdexcept>

typedef void (*SoftSusyBCFnPtr)(MssmSoftsusy &, const DoubleVector &);

class softsusy_opts {
public:

    softsusy_opts();

    SoftSusyBCFnPtr get_bcs() const;

    DoubleVector get_pars() const { return pars; }

    double get_tb() const { return tb; }

    int get_sgnmu() const { return sgnmu; }

    double pars_at(int i) const { return pars(i); }

    double get_mgut_guess() const { return mgut_guess; }

    double get_alpha_s() const { return alpha_s; }

    double get_alpha_em_inv() const { return alpha_em_inv; }

    double get_mtop_pole() const { return mtop_pole; }

    double get_mbmb() const { return mbmb; }

    void set_pars(const DoubleVector &x) {
        pars = x;
        nusugra = (pars.displayEnd() != 3); // if 3, we have mSUGRA
    }

    void set_tb(double x) { tb = x; }

    void set_sgnmu(int x) { sgnmu = x; }

    void set_mgut_guess(double x) { mgut_guess = x; }

    void set_alpha_s(double x) { alpha_s = x; }

    void set_alpha_em_inv(double x) { alpha_em_inv = x; }

    void set_mtop_pole(double x) { mtop_pole = x; }

    void set_mbmb(double x) { mbmb = x; }

    bool is_nusugra() const { return nusugra; }

    constexpr static double default_tb = 15;
    constexpr static int default_sgnmu = 1;

    constexpr static double default_mgut = 2e16;
    constexpr static double default_alpha_s = 0.1184;
    constexpr static double default_alpha_em_inv = 127.933;
    constexpr static double default_mtop = 173.07;
    constexpr static double default_mbmb = 4.18;

protected:

    DoubleVector pars;
    double tb;
    int sgnmu;

    double mgut_guess;
    double alpha_s;
    double alpha_em_inv;
    double mtop_pole;
    double mbmb;

    bool nusugra;

};

#endif


