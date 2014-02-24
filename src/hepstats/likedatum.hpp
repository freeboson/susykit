

#pragma once
#ifndef LIKEDATUM_H
#define LIKEDATUM_H

#include <cmath>
#include "model.h"
#include "model_lookup.h"

class hepstats::likedatum
{
public:
	likedatum() :unset(true) {}
	likedatum(	likedist::distribution _dist,
			model_lookup _lookup, 
			double _exp_value, 
			double _exp_uncertainty, 
			double _theory_uncertainty, 
			bool _theory_percent_error = false
		)

	:	dist(_dist),
		lookup(_lookup),
		exp_value(_exp_value),
		exp_uncertainty(_exp_uncertainty),
		theory_uncertainty(_theory_uncertainty),
		theory_percent_error(_theory_percent_error),
		unset(false)

	{
		// all done!
	}

	double operator()(const model &m, bool *unlikely = nullptr) const; // note that this gives -ln(like) = chi^2/2!!!!

private:
	likedist::distribution dist;
	model_lookup lookup; 
	double exp_value; 
	double exp_uncertainty; 
	double theory_uncertainty; 
	bool theory_percent_error;

	bool unset;

	double get_95cl_loglike(double delta, bool *unlikely) const;
	double z_fn(double arg) const;
};

#endif

