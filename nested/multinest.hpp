/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - multinest.hpp:                                                           -
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
#ifndef MULTINEST_HPP
#define MULTINEST_HPP

#include <array>
#include <memory>
#include "softsusy_opts.hpp"
#include "priors.hpp"
#include "hepstats.hpp"


namespace nested {


    // need singleton pattern here

    template<std::size_t ndim>
    class multinest : public softsusy_opts {

    public:

        typedef std::array<std::unique_ptr<priors::prior>, ndim> prior_list;

        multinest(prior_list _param_priors, hepstats::loglike _loglike_calc);

    private:

        prior_list param_priors;
        hepstats::loglike loglike_calc;

    };

}

template<std::size_t ndim>
nested::multinest<ndim>::multinest(prior_list param_priors, hepstats::loglike _loglike_calc)
        : param_priors(_param_priors), loglike_calc(_loglike_calc) {

}

#endif 

