/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - interpolated_data.hpp:                                                   -
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
#ifndef SUSYKIT_INTERPOLATED_DATA_HPP
#define SUSYKIT_INTERPOLATED_DATA_HPP

#include <string>
#include <vector>
#include <utility>
#include <istream>
#include "constrain/likedatum.hpp"
#include "experimental_data.hpp"

namespace hepstats {
    class interpolated_data : public experimental_data {
    public:
        interpolated_data(model_lookup _lookup_axis,
                          const std::string &data_filename,
                          double _limit_error)
                : lookup_axis(_lookup_axis), limit_error(_limit_error) {
            load_data(data_filename);
        }

        interpolated_data(model_lookup _lookup_axis,
                          double _limit_error,
                          std::istream *is);

        double get_limit(const model &m) const override;

        double get_limit_error(const model &) const override {
            return limit_error;
        }

    private:
        void load_data(const std::string &data_filename);

        void load_data(std::istream *is);

        const model_lookup lookup_axis;
        const double limit_error;
        constexpr size_t table_start_size = 500;
        std::vector<std::pair<double, double> > table;
    };
}

#endif //SUSYKIT_INTERPOLATED_DATA_HPP

