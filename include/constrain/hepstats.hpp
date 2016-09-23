/*

    ****************************************************************************
    *                                                                          *
    *                                                                          *
    * Sujeet Akula                                                             *
    * sujeet@freeboson.org                                                     *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    ****************************************************************************

*/


#pragma once
#ifndef HEPSTATS_HPP
#define HEPSTATS_HPP

namespace hepstats {
    class loglike;

    class likedatum;

    class likeconfig;

    namespace likedist {
        enum distribution {
            gaussian,
            poisson,
            upper,
            lower,
            upper_gaussian,
            upper_interpolated,
            lower_interpolated
        };
    }

    const double logZero = -1e90; // embarrassing

    const double br_btaunu_sm = 1.18e-4;
    const double br_btaunu_sm_err = 0.16e-4;
}

#include "loglike.hpp"
#include "likedatum.hpp"
#include "likeconfig.hpp"

#endif

