
#pragma once
#ifndef SUJSOFT_HPP
#define SUJSOFT_HPP

#include "softsusy_opts.hpp"
#include "libconstrain.h"

class softsusy_driver
{
public:
	softsusy_driver(const softsusy_opts * const _sugra);

	model operator() (bool gauge_unification = true);

private:

	void init_qedqcd();
	double run();

	model slha_to_model(const double &mGUT);

	const softsusy_opts *sugra;
	QedQcd oneset;
	MssmSoftsusy rge;
};

#endif


