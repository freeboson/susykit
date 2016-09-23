/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - constraint.hpp:                                                          -
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
#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP

#include <iostream>
#include <string>
#include <sstream>

#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"

#include "constrain/hepstats.hpp"

class constraint {
public:

    constraint();

    constraint(const model_lookup &_ml, const std::string &cons_line);

    constraint(const hepstats::loglike &_loglike, const std::string &limit);

    int operator()(const model &m) const;

    double get_value(const model &m) const;

    std::string get_constraint() const;

private:
    model_lookup ml;
    hepstats::loglike loglike;
    std::string param;
    double lower, upper;
    bool lset, uset;
    bool xset;
    bool like_constraint;

    void process_limit(std::stringstream &ss);

    int check_model(double value) const;
};

#endif

