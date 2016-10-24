/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - likedatum.hpp:                                                           -
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
#ifndef LIKEDATUM_HPP
#define LIKEDATUM_HPP

#include <cmath>
#include "model.hpp"
#include "model_lookup.hpp"
#include "experimental_data.hpp"

namespace hepstats {
    class likedatum {
    public:
        likedatum(
                model_lookup _lookup,
                double _pred_error, bool _pred_percent_error,
                std::unique_ptr<experimental_data> _exp)
                : lookup(_lookup),
                  pred_error(_pred_error),
                  pred_percent_error(_pred_percent_error),
                  exp_data(std::move(_exp)) {
            // all done!
        }

        // NOTE: per sampler convention, this gives -ln(like) = chi^2/2
        double operator()(const model &m,
                          bool *unlikely = nullptr) const;

        virtual double calculate_pull(double pred, double limit,
                                      double tau, double sigma,
                                      bool *unlikely) const = 0;

    private:
        model_lookup lookup;
        std::unique_ptr<experimental_data> exp_data;

        double pred_error;
        bool pred_percent_error;

    };
}

#endif

