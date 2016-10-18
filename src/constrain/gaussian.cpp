/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - gaussian_datum.cpp:                                                      -
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
#include "gaussian.hpp"

double
hepstats::gaussian::calculate_pull(const double &pred, const double &limit,
                                   const double &limit_error,
                                   bool *unlikely) const override {
    *unlikely = false;
    double tau = theory_percent_error ? pred*theory_error : theory_error;
    return -std::pow(pred - limit, 2.0) /
           (std::pow(tau, 2.0) + std::pow(limit_error, 2.0)) / 2.0;
}

