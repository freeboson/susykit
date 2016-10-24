/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - smeared_upper_limit.hpp:                                                 -
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
#ifndef SUSYKIT_SMEARED_UPPER_LIMIT_HPP
#define SUSYKIT_SMEARED_UPPER_LIMIT_HPP


#include "smeared_limit.hpp"

namespace hepstats {
    class smeared_upper_limit : public smeared_limit {
    public:
        smeared_upper_limit(model_lookup lookup,
                            double pred_error, bool pred_percent_error,
                            std::unique_ptr<experimental_data> data)
                : smeared_limit(lookup, pred_error, pred_percent_error,
                                move(data)) {}

        inline double get_delta(double pred, double limit) final {
            return pred - limit;
        }
    };
}


#endif //SUSYKIT_SMEARED_UPPER_LIMIT_HPP
