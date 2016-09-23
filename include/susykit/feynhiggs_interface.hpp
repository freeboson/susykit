/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - feynhiggs_interface.hpp:                                                 -
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
#ifndef FEYNHIGGS_INTERFACE_HPP
#define FEYNHIGGS_INTERFACE_HPP

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"
#include "constrain/parse.hpp"
#include "feynhiggs/CFeynHiggs.h"

// this class will implement thread-local
// storage to allow parallel calls to FeynHiggs
// ... in the future

#define FH_SLHAData_len 5558

class feynhiggs_driver {
public:
    feynhiggs_driver();

    void operator()(model *m);

//	~feynhiggs_driver();

    static constexpr unsigned int slhadata_len = FH_SLHAData_len;
private:
    int mssm_scope;
    int field_renorm;
    int tanbeta_renorm;
    int higgs_mixing;
    int p2_approx;
    int loop_level;
    int run_mt;
    int bottom_resum;
    int two_loop_complex_approx;
    int debug_level;

    int fh_error_state;

    COMPLEX slha[slhadata_len];    // COMPLEX is defined in ftypes.h

    void calc_observables(model *m);

    void pass_feynhiggs_slha_data(const model *m);
};

#endif


