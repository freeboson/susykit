/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - superiso_interface.hpp:                                                  -
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
#ifndef SUPERISO_INTERFACE_HPP
#define SUPERISO_INTERFACE_HPP


#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"
#include "constrain/parse.hpp"
#include <memory>

struct parameters;

class superiso_driver {
public:
    superiso_driver();

    void operator()(model *m);

private:

    void pass_superiso_slha_data(const model &m);

    void calc_observables(model *m);

    void init_params();

    void slha_adjust();

    double bsgamma();

    double bsmumu();

    double btaunu();

    double gmuon();

#ifndef SUPERISO_NO_RELIC

    double relic_density();

#endif

    std::shared_ptr<parameters> superiso_params;
};

#endif


