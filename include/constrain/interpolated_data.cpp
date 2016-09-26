/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - interpolated_data.cpp:                                                   -
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


#include "interpolated_data.hpp"

#include <fstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

bool first_coord_lt(const auto &lhs, const auto &rhs) {
    return std::get<0>(lhs) < std::get<0>(rhs);
}

bool first_coord_eq(const auto &lhs, const auto &rhs) {
    return std::get<0>(lhs) == std::get<0>(rhs);
}

double hepstats::interpolated_data::get_limit_at(const double &x) const {
    if (x <= std::get<0>(table.front()))
        return std::get<1>(table.front());
    if (x >= std::get<0>(table.back()))
        return std::get<1>(table.back());
    const auto upper = std::lower_bound(begin(table), end(table),
                                        first_coord_lt);
    const auto lower = std::prev(upper);
    double dx = upper->first - lower->first;
    double dy = upper->second - lower->second;
    if (dx == 0.0) return lower->first;
    return (dy/dx)*(x - lower->first) + lower->first;
}

void hepstats::interpolated_data::load_data(const std::string &data_filename) {
    std::ifstream f(data_filename);
    if (f.fail()) {
        throw std::invalid_argument("Could not read from " + data_filename);
    }

    table.reserve(table_start_size);
    double x, y;
    while (f >> x >> y) {
        table.push_back(std::make_pair(x, y));
        if (table.size() >= table.capacity())
            table.reserve(table.capacity()+table_start_size);
    }
    table.shrink_to_fit();
    std::sort(std::begin(table), std::end(table), first_coord_lt);
    std::unique(std::begin(table), std::end(table), first_coord_eq);
}

