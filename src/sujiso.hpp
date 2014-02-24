
#pragma once
#ifndef SUJISO_HPP
#define SUJISO_HPP

#include "libconstrain.h"

//extern "C" {
//#include "include.h"
//}
struct parameters;


class superiso_driver
{
public:
	superiso_driver();

	void operator() (model *m);

	~superiso_driver();
private:

	void pass_superiso_slha_data(const model &m);
	void calc_observables(model *m);

	parameters *superiso;
};

#endif


