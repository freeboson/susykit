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

using coord = hepstats::interpolated_data::coord;

bool first_coord_lt(const coord &lhs, const coord &rhs) {
    return std::get<0>(lhs) < std::get<0>(rhs);
}

bool first_coord_eq(const coord &lhs, const coord &rhs) {
    return std::get<0>(lhs) == std::get<0>(rhs);
}

hepstats::interpolated_data::interpolated_data(model_lookup _lookup_axis,
                                               double _limit_error,
                                               std::istream *is)
        : lookup_axis(_lookup_axis), limit_error(_limit_error) {
    if (is->fail()) {
        throw std::invalid_argument("Could not read from istream object");
    }
    load_data(is);
}

double hepstats::interpolated_data::get_limit(const model &m) const {
    double x = lookup_axis(m);
    if (x <= std::get<0>(table.front()))
        return std::get<1>(table.front());
    if (x >= std::get<0>(table.back()))
        return std::get<1>(table.back());
    const auto upper = std::lower_bound(begin(table), end(table),
                                        coord({x,0.0}), first_coord_lt);
    const auto lower = std::prev(upper);
    double dx = std::get<0>(*upper) - std::get<0>(*lower);
    double dy = std::get<1>(*upper) - std::get<1>(*lower);
    if (dx == 0.0) return std::get<0>(*lower); // should actually throw
    return (dy / dx) * (x - std::get<0>(*lower)) + std::get<1>(*lower);
}

void hepstats::interpolated_data::load_data(const std::string &data_filename) {
    std::ifstream f(data_filename);
    if (f.fail()) {
        throw std::invalid_argument("Could not read from " + data_filename);
    }
    load_data(&f);
}

void hepstats::interpolated_data::load_data(std::istream *is) {
    table.reserve(table_start_size);
    double x, y;
    while ((*is) >> x >> y) {
        table.push_back({x, y});
        if (table.size() >= table.capacity())
            table.reserve(table.capacity() + table_start_size);
    }
    std::sort(std::begin(table), std::end(table), first_coord_lt);
    auto last = std::unique(std::begin(table), std::end(table), first_coord_eq);
    table.erase(last,end(table));
    table.shrink_to_fit();
}

