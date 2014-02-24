

#pragma once
#ifndef POINT_OPTS_HPP
#define POINT_OPTS_HPP

#include "softsusy.h"
#include "linalg.h"

typedef void (*SoftSusyBCFnPtr) (MssmSoftSusy &, const DoubleVector&);

class point_opts
{
public:
	point_opts();

	DoubleVector get_pars() const;

	SoftSusyBCFnPtr get_bcs() const; 

	double get_tb() const;
	int get_sgnmu() const;
private:
	bool nu_gaugino;
	bool nu_higgs;
	bool nu_trilinear;
	bool nu_3rdgen;
	bool nu_mq_ml;
	bool nu_full_sugra;
};

#endif


