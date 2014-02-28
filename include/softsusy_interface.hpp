

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
#ifndef SUJSOFT_HPP
#define SUJSOFT_HPP

#include "softsusy_opts.hpp"
#include "libconstrain.hpp"

class softsusy_driver
{
public:
	softsusy_driver(const softsusy_opts * const _sugra);

#if 0
	// this member function will ignore any differences between
	// _sugra and sugra when it comes to alpha_s, alpha_em, mb(mb)
	// and m_top(pole) -- it only changes the pars DoubleVector, 
	// tan(beta) and sgnmu.
	void replace_soft_terms(const softsusy_opts * const _sugra);
#endif

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


