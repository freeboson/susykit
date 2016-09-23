/*

    ****************************************************************************
    *                                                                          *
    *                                                                          *
    * Sujeet Akula                                                             *
    * sujeet@freeboson.org                                                     *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    ****************************************************************************

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


