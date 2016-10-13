/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - hepstats.hpp:                                                            -
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
#ifndef HEPSTATS_HPP
#define HEPSTATS_HPP

namespace hepstats {
    class loglike;

    class likedatum;

    class likeconfig;

    enum class likedist {
        gaussian,
//        poisson,
        upper,
        lower,
        upper_gaussian,
        upper_interpolated,
        lower_interpolated
    };

    constexpr double logZero = -1e90; // embarrassing

    constexpr double br_btaunu_sm = 1.18e-4;
    constexpr double br_btaunu_sm_err = 0.16e-4;
}

#include "loglike.hpp"
#include "likedatum.hpp"
#include "likeconfig.hpp"

#endif

