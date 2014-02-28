
#define SUPERISO_INTERFACE_PIMPL

extern "C" {
  #include "include.h" // terrible name
}

#include "superiso_interface.hpp"

void superiso_pimpl::init_params(parameters *params)
{
	Init_param(params);
}

void superiso_pimpl::slha_adjust(parameters *params)
{
	slha_adjust(params);
}

double superiso_pimpl::bsgamma(parameters *params)
{
	double C0w[11],C1w[11],C2w[11],C0b[11],C1b[11],C2b[11];
	
	double mu_W=2.* params->mass_W;
	double mu_b=params->mass_b_1S/2.;

	CW_calculator(C0w,C1w,C2w,mu_W,params);
	C_calculator_base1(C0w,C1w,C2w,mu_W,C0b,C1b,C2b,mu_b,params);
	return ::bsgamma(C0b,C1b,C2b,mu_b,mu_W,params);
}

double superiso_pimpl::bsmumu(parameters *params)
{	
	double C0b[11],C1b[11],C0w[11],C1w[11],C2w[11],Cpb[11];
	double complex CQ0b[3],CQ1b[3],CQpb[3];
	
	double mu_W=2.*params->mass_W;
	double mu_b=params->mass_b;
				
	CW_calculator(C0w,C1w,C2w,mu_W,params);
	C_calculator_base2(C0w,C1w,mu_W,C0b,C1b,mu_b,params);
	CQ_calculator(CQ0b,CQ1b,mu_W,mu_b,params);
	Cprime_calculator(Cpb,CQpb,mu_W,mu_b,params);

	return Bsmumu(C0b,C1b,CQ0b,CQ1b,Cpb,CQpb,params,mu_b);
}

double superiso_pimpl::btaunu(parameters *params)
{	
	return Btaunu(params);
}

double superiso_pimpl::gmuon(parameters *params)
{	
	return muon_gm2(params);
}

double superiso_pimpl::relic_density(parameters *params)
{

	double Wefftab[NMAX][2];
	int nlines_Weff;

	struct relicparam paramrelic;

	Init_relic(Wefftab,&nlines_Weff,params);
	Init_cosmomodel(&paramrelic);

	return ::relic_density(Wefftab,nlines_Weff,params,&paramrelic);
}


#undef SUPERISO_INTERFACE_PIMPL

