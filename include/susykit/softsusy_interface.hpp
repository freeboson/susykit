/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - softsusy_interface.hpp:                                                  -
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
#ifndef SOFTSUSY_INTERFACE_HPP
#define SOFTSUSY_INTERFACE_HPP

#include "softsusy_opts.hpp"
#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"
#include "constrain/parse.hpp"

class softsusy_driver {
public:
    softsusy_driver(const softsusy_opts *const _sugra);

#if 0
    // this member function will ignore any differences between
    // _sugra and sugra when it comes to alpha_s, alpha_em, mb(mb)
    // and m_top(pole) -- it only changes the pars DoubleVector,
    // tan(beta) and sgnmu.
    void replace_soft_terms(const softsusy_opts * const _sugra);
#endif

    model operator()(bool gauge_unification = true);

private:

    void init_qedqcd();

    double run();

    model slha_to_model(const double &mGUT);

    const softsusy_opts *sugra;
    QedQcd oneset;
    MssmSoftsusy rge;
};

#endif


