

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


#include "libconstrain.hpp"
#include <memory>

struct parameters;

class superiso_driver
{
public:
	superiso_driver();

	void operator() (model *m);

private:

	void pass_superiso_slha_data(const model &m);
	void calc_observables(model *m);

	void init_params();
	void slha_adjust();

	double bsgamma();
	double bsmumu();
	double btaunu();
	double gmuon();
	double relic_density();

	std::shared_ptr<parameters> superiso_params;
};

#endif


