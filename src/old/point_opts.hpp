

#pragma once
#ifndef POINT_OPTS_HPP
#define POINT_OPTS_HPP

#include "softsusy.h"
#include "linalg.h"

#include <stdexcept>

typedef void (*SoftSusyBCFnPtr) (MssmSoftsusy &, const DoubleVector&);

class point_opts
{
public:
	point_opts(int argc, char**argv);

	SoftSusyBCFnPtr get_bcs() const; 

	DoubleVector get_pars() const { return pars; }
	double get_tb() const { return tb; }
	int get_sgnmu() const { return sgnmu; }

	double get_mgut_guess() const { return mgut_guess; }
	double get_alpha_s() const { return alpha_s; }
	double get_alpha_em_inv() const { return alpha_em_inv; }
	double get_mtop_pole() const { return mtop_pole; }
	double get_mbmb() const { return mbmb; }

	bool is_nusugra() const { return nusugra; }

	void usage() const;

private:

	int opthandle(int argc, char** argv);
	void make_pars(int argi, char** argv);

	bool nusugra;
	bool gaugino;
	bool higgs;
	bool trilinear;
	bool thirdgen;
	bool mq_ml;
	bool full_sugra;

	DoubleVector pars;
	double tb;
	int sgnmu;

	double mgut_guess;
	double alpha_s;
	double alpha_em_inv;
	double mtop_pole;
	double mbmb;
};

#endif


