/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - simple_datum.hpp:                                                        -
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
#ifndef SUSYKIT_SIMPLE_DATUM_HPP
#define SUSYKIT_SIMPLE_DATUM_HPP

#include "experimental_data.hpp"

namespace hepstats {
    class simple_datum : public experimental_data {
    public:
        simple_datum(double _limit, double _limit_error)
                : limit(_limit), limit_error(_limit_error) {}

        inline double get_limit(const model &) const override { return limit; }
        inline double get_limit_error(const model &) const override {
            return limit_error;
        }

    private:
        const double limit, limit_error;
    };
}

#endif //SUSYKIT_SIMPLE_DATUM_HPP

