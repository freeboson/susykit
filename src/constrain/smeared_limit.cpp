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
#include "smeared_limit.hpp"

double z_fn(double arg) {
    return std::erfc(arg / sqrt(2.0)) / 2.0;
}

double hepstats::smeared_limit::get_95cl_loglike(double delta,
                                                 double exp_uncertainty,
                                                 double tau,
                                                 bool *unlikely) const {
    // this is based on SmearedBound() in SuperBayes, source/calclike.f90
    // which in turn is based on hep-ph/0602028, with a minor fix
    double sigma = (exp_uncertainty <= 0.0) ? (1e-2 * tau) : exp_uncertainty;
    double error = std::hypot(sigma, tau);
    double tlim = (sigma / tau) * delta / error;

    double expterm = std::exp(-std::pow(delta,2.0) /
                                      (2.0*std::pow(error,2.0)));

    double zterm2 = z_fn(delta / tau);

    double loglikeval = 0.0;

    if (expterm > 0.0) {
        loglikeval = std::log(
                sigma / delta * expterm * (1 - z_fn(tlim)) + zterm2);
    } else {
        if (zterm2 < 0.5) {
//			std::cerr << "unlikely!" << std::endl;
            *unlikely = true;
            loglikeval = logZero;
        } else {
            loglikeval = 0.0;
        }
    }
    return loglikeval;
}


