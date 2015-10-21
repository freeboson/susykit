
#include "libsusykit/softsusy_opts.hpp"

softsusy_opts::softsusy_opts()
:pars(0),tb(default_tb),sgnmu(default_sgnmu),mgut_guess(default_mgut),alpha_s(default_alpha_s),alpha_em_inv(default_alpha_em_inv),mtop_pole(default_mtop),mbmb(default_mbmb)
{

	// leaves uninitialized: tb, sgnmu, nusugra

}


SoftSusyBCFnPtr softsusy_opts::get_bcs() const
{
	if (nusugra)
		return extendedSugraBcs;
	else
		return sugraBcs;

}


