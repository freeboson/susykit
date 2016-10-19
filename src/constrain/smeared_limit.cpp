/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - smeared_limit.cpp:                                                       -
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

#include <cmath>
#include <stdexcept>
#include "smeared_limit.hpp"

double z_fn(double arg) {
    return std::erfc(arg / sqrt(2.0)) / 2.0;
}

double get_delta_upper(double limit, double pred) {
    return std::abs(pred) - limit;
}

double get_delta_lower(double limit, double pred) {
    return limit - std::abs(pred);
}

hepstats::smeared_limit::smeared_limit(model_lookup ml, double theory_error,
                                       bool theory_percent_error, limit_type lt)
        : likedatum(ml, theory_error, theory_percent_error) {
    if (upper == limit_type) {
        get_delta = &get_delta_upper;
    } else if (lower == limit_type) {
        get_delta = &get_delta_lower;
    } else {
        throw (std::logic_error("Unknown limit_type!"));
    }
}

double hepstats::smeared_limit::get_95cl_loglike(double limit,
                                                 double sigma,
                                                 bool *unlikely) const {
    // this is based on SmearedBound() in SuperBayes, source/calclike.f90
    // which in turn is based on hep-ph/0602028, with a minor fix
    double sigma = (exp_uncertainty <= 0.0) ? (1e-2 * tau) : exp_uncertainty;
    double error = std::hypot(sigma, tau);
    double tlim = (sigma / tau) * delta / error;

    double error = hypot(sigma,tau);

    double expterm = exp(-0.5 * pow(-delta/error,2.0));
    double zterm2 = erfc((delta/tau) / sqrt(2.0)) / 2.0;

    cout << "  dbg:  zfunc2 = " << zterm2 << nl
         << "  dbg: z_tstar = " << (1+erf((sigma/tau)*delta/error/sqrt(2.0)))/2.0 <<endl;

    if (expterm > 0.0) {
        return -log(sigma/error * expterm * (1+erf((sigma/tau)*delta/error/sqrt(2.0)))/2.0 + zterm2);
    } else {
        if (zterm2 < 0.5)
            return -1e25;
        else
            return 0.0;
    }
}


