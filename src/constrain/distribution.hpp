/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - distribution.hpp:                                                        -
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
#ifndef SUSYKIT_DISTRIBUTION_HPP
#define SUSYKIT_DISTRIBUTION_HPP


namespace hepstats {
    class distribution {
    public:
        distribution(double _theory_error, bool _theory_percent_error)
                : theory_error(_theory_error),
                  theory_percent_error(_theory_percent_error) {}

        virtual double calculate_pull(double pred, double limit,
                                      double limit_error,
                                      bool *unlikely) const = 0;

        const double theory_error;
        const bool theory_percent_error;
    };
}


#endif //SUSYKIT_DISTRIBUTION_HPP
