/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - likedatum.cpp:                                                           -
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


#include "constrain/hepstats.hpp"

double hepstats::likedatum::operator()(const model &m, bool *unlikely) const {
    double theoretical_value = lookup(m);

    if (lookup.get_mode() == model_lookup::output
        && lookup.get_code() == susy_dict::observable::btaunu) { // special case
        theoretical_value *= br_btaunu_sm; // the btaunu value is SUSY/SM ratio
    }

    double tau = (theory_percent_error)
                 ? theoretical_value * theory_uncertainty
                 : theory_uncertainty;

    return calculate_pull(theoretical_value, tau, unlikely);

    double loglikeval = 0.0;
    *unlikely = false;

    double delta;

    switch (dist) {
        case likedist::Gaussian:
            loglikeval = -std::pow(theoretical_value - exp_value, 2.0) /
                         (std::pow(tau, 2.0) + std::pow(exp_uncertainty, 2.0)) / 2.0;
            break;

            // leave these alone for now
        case likedist::Upper:
        case likedist::Lower:
            delta = (likedist::Lower == dist) ? (exp_value - std::abs(theoretical_value)) : (
                    std::abs(theoretical_value) - exp_value);
            if (tau <= 0.0) // this is borked... do a step-fn
            {
                if (delta > 0.0) // remember we are making it like a lower-bound
                {
                    if (nullptr != unlikely)
                        *unlikely = true;
//					std::cerr << "Unlikely! " << lookup.get_code() << "=" << theoretical_value << std::endl;
                    loglikeval = logZero;
                }
            }
            else {
                loglikeval = get_95cl_loglike(delta, tau, unlikely);
            }
            break;

        case likedist::Upper_Gaussian:
            if (theoretical_value > exp_value)
                loglikeval = -std::pow(theoretical_value - exp_value, 2.0) /
                             (std::pow(tau, 2.0) + std::pow(exp_uncertainty, 2.0)) / 2.0;
            break;

        default:
            std::cerr << "This type of distribution is not available!" << std::endl;
            return -1;
    }
    if (false == *unlikely && loglikeval < -15.0) { ;
//		std::cerr << "Whoa: " << lookup.get_code() << "=" << theoretical_value << std::endl;
    }

    return loglikeval;
}

double hepstats::likedatum::get_95cl_loglike(double delta, double tau, bool *unlikely) const {
    // this is based on SmearedBound() in SuperBayes, source/calclike.f90
    // which in turn is based on hep-ph/0602028, with a minor fix
    double sigma = (exp_uncertainty <= 0.0) ? (1e-2 * tau) : exp_uncertainty;
    double quad_err = std::hypot(sigma, tau);
    double tlim = (sigma / tau) * delta / quad_err;

    double exparg = -delta * delta / (2.0 * quad_err * quad_err);
    double expterm = std::exp(exparg);

    double zterm2 = z_fn(delta / tau);

    double loglikeval = 0.0;

    if (expterm > 0.0) {
        loglikeval = std::log(sigma / quad_err * expterm * (1 - z_fn(tlim)) + zterm2);
    }
    else {
        if (zterm2 < 0.5) {
//			std::cerr << "unlikely!" << std::endl;
            *unlikely = true;
            loglikeval = logZero;
        }
        else {
            loglikeval = 0.0;
        }
    }
    return loglikeval;
}

double hepstats::likedatum::z_fn(double arg) const {
    return std::erfc(arg / sqrt(2.0)) / 2.0;
}


