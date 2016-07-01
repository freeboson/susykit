

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
#ifndef SUPERISO_INTERFACE_HPP
#define SUPERISO_INTERFACE_HPP


#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"
#include "constrain/parse.hpp"
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
#ifndef SUPERISO_NO_RELIC
	double relic_density();
#endif

	std::shared_ptr<parameters> superiso_params;
};

#endif


