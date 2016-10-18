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

namespace hepstats {
    class likedatum {
    public:
        likedatum(
                model_lookup _lookup,
                double _theory_uncertainty, bool _theory_percent_error)
                : lookup(_lookup),
                  theory_uncertainty(_theory_uncertainty),
                  theory_percent_error(_theory_percent_error) {
            // all done!
        }

        // NOTE: per sampler convention, this gives -ln(like) = chi^2/2
        double operator()(const model &m,
                          bool *unlikely = nullptr) const;

    private:
        model_lookup lookup;
        std::shared_pointer<experimental_data> exp;
        std::shared_pointer<distribution> dist;

        double theory_uncertainty;
        bool theory_percent_error;

    };
}

#endif

