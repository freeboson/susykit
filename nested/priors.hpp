/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - priors.hpp:                                                              -
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
#ifndef PRIORS_HPP
#define PRIORS_HPP

namespace priors {

    class prior {

    public:

        prior(double _min, double _max)
                : min(_min), max(_max) {
            diff = max - min;
        }

        virtual double map(double variate) const = 0;

        double get_min() const { return min; }

        double get_max() const { return max; }

        double get_diff() const { return diff; }

    private:

        double min, max;
        double diff;

    };

    class linear : public prior {

    public:
        linear(double _min, double _max)
                : prior(_min, _max) {

        }

        double map(double variate) const;
    };

    class log : public prior {

    public:
        log(double _min, double _max)
                : prior(_min, _max) {

        }

        double map(double variate) const;
    };

}


#endif


