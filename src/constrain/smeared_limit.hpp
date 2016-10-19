/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - smeared_limit.hpp:                                                       -
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
#ifndef SUSYKIT_SMEARED_LIMIT_HPP
#define SUSYKIT_SMEARED_LIMIT_HPP

#include "constrain/hepstats.hpp"

namespace hepstats {
    class smeared_limit : public likedatum {
    public:
        smeared_limit(model_lookup lookup,
                      double pred_error, bool pred_percent_error)
                : likedatum(lookup, pred_error, pred_percent_error) {}

        virtual double get_delta(double pred, double limit) const = 0;

        double calculate_pull(double pred, double limit,
                              double tau, double sigma,
                              bool *unlikely) const override;
    };
}

#endif //SUSYKIT_SMEARED_LIMIT_HPP
