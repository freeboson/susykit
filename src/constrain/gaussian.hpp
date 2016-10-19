/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - gaussian.hpp:                                                            -
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
#ifndef SUSYKIT_GAUSSIAN_HPP
#define SUSYKIT_GAUSSIAN_HPP

#include "constrain/likedatum.hpp"

namespace hepstats {
    class gaussian : public likedatum {
    public:
        gaussian(model_lookup lookup,
                 double pred_error, bool pred_percent_error)
                : likedatum(lookup, pred_error, pred_percent_error) {}

        virtual double calculate_pull(double pred, double limit,
                                      double tau, double sigma,
                                      bool *unlikely) const override;
    };
}

#endif //SUSYKIT_GAUSSIAN_HPP

