

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

#pragma once
#ifndef SUJISO_HPP
#define SUJISO_HPP


struct parameters;

class superiso_pimpl
{
public:
	static void init_params(parameters *params);
	static void slha_adjust(parameters *params);

	static double bsgamma(parameters *params);
	static double bsmumu(parameters *params);
	static double btaunu(parameters *params);
	static double gmuon(parameters *params);

	static double relic_density(parameters *params);
private:

};

// do not show C++11 to pimpl
#ifndef SUPERISO_INTERFACE_PIMPL

#include "libconstrain.hpp"
#include <memory>

class superiso_driver
{
public:
	superiso_driver();

	void operator() (model *m);

private:

	void pass_superiso_slha_data(const model &m);
	void calc_observables(model *m);

	std::shared_ptr<parameters> superiso_params;
	std::unique_ptr<superiso_pimpl> superiso;
};
#endif

#endif


