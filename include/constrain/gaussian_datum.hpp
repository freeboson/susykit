/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - gaussian_datum.hpp:                                                      -
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
#ifndef SUSYKIT_GAUSSIAN_DATUM_HPP
#define SUSYKIT_GAUSSIAN_DATUM_HPP

#include "hepstats.hpp"

namespace hepstats {

    class gaussian_datum : public likedatum {
    public:
        gaussian_datum(const model_lookup &_lookup, double _theory_uncertainty,
                       bool _theory_percent_error, double exp_value,
                       double exp_uncertainty)
                : likedatum(_lookup,
                            _theory_uncertainty,
                            _theory_percent_error),
                  exp_value(exp_value), exp_uncertainty(exp_uncertainty) { }

    private:
        double calculate_pull(const double &theoretical_value,
                              const double &tau, bool *unlikely) const;

        double exp_value;
        double exp_uncertainty;
    };

}

#endif //SUSYKIT_GAUSSIAN_DATUM_HPP
