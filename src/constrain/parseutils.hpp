/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - parseutils.hpp:                                                          -
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
#ifndef PARSEUTILS_HPP
#define PARSEUTILS_HPP

#include <ostream>
#include <vector>
#include <iomanip>

#include "constrain/model.hpp"

#define DEFAULT_COL_WIDTH 12

std::ostream &operator<<(std::ostream &o, const std::vector<double> &v);

class row_print {
public:
    row_print(std::ostream *_o) : o(_o) { }

    void operator()(const std::vector<double> &v) const {
        *o << v << std::endl;
    }

private:
    std::ostream *o;
};

class row_printer {
public:
    row_printer(std::ostream *_o) : o(_o) { }

    void operator()(const double &d) const {
        *o << std::setw(DEFAULT_COL_WIDTH) << std::setiosflags(std::ios::left) << d << " ";
    }

private:
    std::ostream *o;
};

#endif


