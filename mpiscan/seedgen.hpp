/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - seedgen.hpp:                                                             -
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
#ifndef SEEDGEN_HPP
#define SEEDGEN_HPP

#include <vector>
#include <random>

class seedgen {
public:

    using seed_type std::random_device::result_type;

    seedgen(unsigned int _num_seeds);

    seed_type operator()();

#ifndef BOOST_MPI

    seed_type *get();

#endif

private:

    void build_seeds();

    unsigned int num_seeds;
    std::vector<seed_type> seeds;
    std::vector<seed_type>::iterator seed_it;

};

#endif


