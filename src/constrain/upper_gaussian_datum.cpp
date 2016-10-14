/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - upper_gaussian_datum.cpp:                                                -
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


#include "upper_gaussian_datum.hpp"

double hepstats::upper_gaussian_datum::calculate_pull(const double
                                                 &theoretical_value,
                                                const double &tau,
                                                bool *unlikely) const {
    *unlikely = false;
    if (theoretical_value <= exp_value)
        return 0.0;
    return -std::pow(theoretical_value - exp_value, 2.0) /
           (std::pow(tau, 2.0) + std::pow(exp_uncertainty, 2.0)) / 2.0;
}

