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
#include "constrain/hepstats.hpp"
#include "smeared_limit.hpp"

using namespace std;

double hepstats::smeared_limit::calculate_pull(double pred, double limit,
                                               double tau, double sigma,
                                               bool *unlikely) const {
    if (nullptr != unlikely) *unlikely = false;
    // this is based on SmearedBound() in SuperBayes, source/calclike.f90
    // which in turn is based on hep-ph/0602028, with a minor fix
    if (sigma <= 0.0)
        sigma = 1e-2 * tau;
    double delta = get_delta(pred, limit);
    double error = std::hypot(sigma, tau);

    double expterm = exp(-0.5 * pow(-delta / error, 2.0));
    double zterm2 = erfc((delta / tau) / sqrt(2.0)) / 2.0;

    // double tlim = (sigma / tau) * delta / error;

    if (isnormal(expterm)) {
        double zterm1 = (1 + erf((sigma*delta)/(tau*error*sqrt(2.0))))/ 2.0;
        double lnlike = log((sigma/error)*expterm*zterm1 + zterm2);
        if (isfinite(lnlike)) {
            return lnlike;
        } else {
            if (nullptr != unlikely) *unlikely = true;
            return logZero;
        }
    } else {
        if (zterm2 < 0.5) {
            if (nullptr != unlikely) *unlikely = true;
            return logZero; // zterm2 == 0
        } else {
            return 0.0;     // zterm2 == 1
        }
    }
}
