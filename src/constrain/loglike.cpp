/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - loglike.cpp:                                                             -
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


#include "constrain/hepstats.hpp"

#include <numeric>


double hepstats::loglike::get_log_like(const model &m) const {
    bool bad_like = false;
    std::vector<double> likelihoods(like_terms.size());
    std::transform(
            like_terms.begin(),
            like_terms.end(),
            likelihoods.begin(),
            [&m, &bad_like](const auto &datum) -> double {
                bool bad_like_term;
                double result = (*datum)(m, &bad_like_term);
                if (bad_like_term)
                    bad_like = true;
                return result;
            }
    );

    if (bad_like)
        return logZero;

    return std::accumulate(likelihoods.begin(), likelihoods.end(), 0.0);
}


