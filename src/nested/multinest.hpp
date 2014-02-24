
#ifndef MULTINEST_HPP
#define MULTINEST_HPP
#pragma once

#include <array>
#include <memory>
#include "softsusy_opts.hpp"
#include "priors.hpp"
#include "hepstats.hpp"


namespace nested
{


	// need singleton pattern here
	
	template<std::size_t ndim>
	class multinest : public softsusy_opts
	{

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
:param_priors(_param_priors), loglike_calc(_loglike_calc)
{

}

#endif 



