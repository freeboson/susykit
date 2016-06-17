

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


double hepstats::loglike::get_log_like(const model &m) const
{
	bool badLike = false;
	std::vector<double> likelihoods(like_terms.size());
	std::transform(
		like_terms.begin(),
		like_terms.end(),
		likelihoods.begin(),
		[&m, &badLike] (const likedatum &datum) -> double {
			bool bad_like_term;
			double result = datum(m, &bad_like_term);
			if (bad_like_term)
				badLike = true;
			return result;
		}
	);

	if (badLike)
		return logZero;

	return std::accumulate(likelihoods.begin(),likelihoods.end(),0.0);
}


