/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - likedatum.hpp:                                                           -
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
#ifndef LIKEDATUM_HPP
#define LIKEDATUM_HPP

#include <cmath>
#include "model.hpp"
#include "model_lookup.hpp"

class hepstats::likedatum {
public:
    likedatum() : unset(true) { }

    likedatum(likedist::distribution _dist,
              model_lookup _lookup,
              double _exp_value,
              double _exp_uncertainty,
              double _theory_uncertainty,
              bool _theory_percent_error = false
    )

            : dist(_dist),
              lookup(_lookup),
              exp_value(_exp_value),
              exp_uncertainty(_exp_uncertainty),
              theory_uncertainty(_theory_uncertainty),
              theory_percent_error(_theory_percent_error),
              unset(false) {
        // all done!
    }

    double operator()(const model &m, bool *unlikely = nullptr) const; // note that this gives -ln(like) = chi^2/2!!!!

private:
    likedist::distribution dist;
    model_lookup lookup;
    double exp_value;
    double exp_uncertainty;
    double theory_uncertainty;
    bool theory_percent_error;

    bool unset;

    double get_95cl_loglike(double delta, double tau, bool *unlikely) const;

    double z_fn(double arg) const;
};

#endif

