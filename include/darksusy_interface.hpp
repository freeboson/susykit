

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
#ifndef DARKSUSY_INTERFACE_HPP
#define DARKSUSY_INTERFACE_HPP

#include "libconstrain.hpp"

class darksusy_driver
{
public:

	darksusy_driver(); // ctor, calls dsinit and clears dsslhacom
	
	int operator() (model* const m); //const ptr to non-const

private:

	void pass_darksusy_slha_data(const model* const m);
	int calc_observables(model* const m);

};


#endif


