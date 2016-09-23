/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - mpiscan.hpp:                                                             -
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
#ifndef MPISCAN_HPP
#define MPISCAN_HPP

#include <fstream>
#include <string>
#include <memory>
#include <random>
#include <tuple>

#include "softsusy_interface.hpp"


class point_builder {
public:

    point_builder(int pars_end);

    std::string mpi_setup(std::fstream *writer);

    std::mt19937_64 &gen() { return *gen_ptr; }

    long double get_time() const { return (curr_time() - time); }

    virtual softsusy_opts operator()() = 0;

    virtual bool filter_point(const model &m) {
        return false; //default -- don't filter
    }

    ~point_builder();

    DoubleVector pars;

private:

    long double curr_time() const;

    std::shared_ptr<std::mt19937_64> gen_ptr;
    long double time;

};

#endif

