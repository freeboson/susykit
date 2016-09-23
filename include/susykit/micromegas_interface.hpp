/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - micromegas_interface.hpp:                                                -
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
#ifndef MICROMEGAS_INTERFACE_HPP
#define MICROMEGAS_INTERFACE_HPP

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"
#include "constrain/parse.hpp"

// this class will implement thread-local
// storage to allow parallel calls to micrOMEGAs
// ... in the future

class micromegas_driver {
public:
    enum relic_mode {
        normal, init
    };

    micromegas_driver() : relic_mode_setting(normal) { }

    micromegas_driver(relic_mode _rm)
            : relic_mode_setting(_rm) { }

    model operator()(model m);

    int operator()(model *m);

private:

    void calc_observables(model *m);

    void pass_micromegas_slha_data(const model &m);

    relic_mode relic_mode_setting;
};

#endif


