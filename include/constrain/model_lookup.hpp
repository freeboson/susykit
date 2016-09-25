/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - model_lookup.hpp:                                                        -
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
#ifndef MODEL_LOOKUP_HPP
#define MODEL_LOOKUP_HPP

#include <string>
#include "model.hpp"

typedef double (model::*ModelLookup)(const std::string &) const;

class model_lookup {
public:
    enum model_map {
        slha,
        output,
        special
    };

    model_lookup() = delete;

    model_lookup(model_map _mode, const std::string &_code);

    model_map get_mode() const { return mode; }

    std::string get_code() const { return code; }

    bool good_mode() const { return (action != NULL || mode == special); }

    double operator()(const model &m) const;

private:
    ModelLookup action;
    model_map mode;
    std::string code;
};

#endif


