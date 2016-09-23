/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - special_lookups.hpp:                                                     -
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
#ifndef SPECIAL_LOOKUPS_HPP
#define SPECIAL_LOOKUPS_HPP

#include <vector>
#include <algorithm>

#include "model.hpp"

class calc_perms {
public:
    // ctors
    calc_perms(std::vector<model>::const_iterator _first, std::vector<model>::const_iterator _last);

    // work horse...
    int operator()(int n) const;

private:
    std::vector<model>::const_iterator first, last;
};

class get_lsq {
public:
    double operator()(const model &m) const;
};

class get_lsl {
public:
    double operator()(const model &m) const;
};

class get_RGG_approx {
public:
    double operator()(const model &m) const;
};

#endif


