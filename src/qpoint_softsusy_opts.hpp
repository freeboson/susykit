

#pragma once
#ifndef QPOINT_SOFTSUSY_OPTS_HPP
#define QPOINT_SOFTSUSY_OPTS_HPP

#include "softsusy_opts.hpp"


class qpoint_opts : public softsusy_opts
{
public:
	qpoint_opts(int argc, char**argv);

	void usage() const;

private:

	int opthandle(int argc, char** argv);
	void make_pars(int argi, char** argv);

	bool gaugino;
	bool higgs;
	bool trilinear;
	bool thirdgen;
	bool mq_ml;
	bool full_sugra;

};

#endif


