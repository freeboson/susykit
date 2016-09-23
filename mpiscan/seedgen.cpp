/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - seedgen.cpp:                                                             -
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


#include "seedgen.hpp"

#include <stdexcept>
#include <unordered_set>
#include <iterator>
#include <algorithm>

using namespace std;

seedgen::seedgen(unsigned int _num_seeds)
        : seeds(_num_seeds), num_seeds(_num_seeds) {
    build_seeds();
    seed_it = begin(seeds);
}

auto seedgen::operator()() -> seed_type {
    if (seeds.end() == seed_it) {
        seed_it = begin(seeds);
        throw out_of_range("Requested too many seeds");
    }
    const auto seed = *seed_it;
    advance(seed_it, 1);
    return seed;
}

void seedgen::build_seeds() {
    // use u_set just to manage uniqueness
    unordered_set<seed_type> temp_seeds;
    temp_seeds.reserve(num_seeds);

    random_device rd;
    while (temp_seeds.size() < num_seeds)
        temp_seeds.insert(rd());

    // keep results in vector for easy
    // access when making MPI calls
    copy(begin(temp_seeds), end(temp_seeds), begin(seeds));
}

#ifndef BOOST_MPI

auto seedgen::get() -> seed_type * {
    if (seeds.end() == seed_it) {
        seed_it = begin(seeds);
        throw out_of_range("Requested too many seeds");
    }
    seed_type *send_seed = &seeds[seed_it - begin(seeds)];
    advance(seed_it, 1);
    return send_seed;

}

#endif


