
#include "superiso_interface.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
//#include <complex.h>
#include <string.h>
#include <strings.h>


#define complex _Complex
#define SUSYKIT_INCLUDED
extern "C" {
  #include "include.h" // terrible name
}


#include <iostream>

using namespace susy_dict;


superiso_driver::superiso_driver()
:superiso_params(new parameters)
{
	init_params();
}

void superiso_driver::operator() (model *m)
{
	pass_superiso_slha_data(*m);
	calc_observables(m);
}

void superiso_driver::calc_observables(model *m)
{
	m->set_observable(observable::bsgnlo, bsgamma());
	m->set_observable(observable::bsmumu, bsmumu());
	m->set_observable(observable::btaunu, btaunu());
	m->set_observable(observable::gmuon, gmuon());

#ifndef SUPERISO_NO_RELIC
	std::cerr << "SuperISO Relic: omega h^2 = " << relic_density() << std::endl;
#endif
}


// this is a useful macro for the lines of a block
// that have matrix elements listed. just pass it 
// the literal root of the matrix variables, and 
// then two literal integers for the row/col
#define matblock(name,i,j) m.get_datum( name ## _ ## i ## j )

void superiso_driver::pass_superiso_slha_data(const model &m)
{
	init_params();

	if (model::mSUGRA == m.get_model_type())
		superiso_params->model = 1;
	else if (model::NUSUGRA == m.get_model_type())
		superiso_params->model = 1; // ???
	else
		return;
	
	// sminputs
	
	superiso_params->inv_alpha_em = m.get_datum(alpha_em_inv);
	superiso_params->Gfermi = m.get_datum(g_fermi);
	superiso_params->alphas_MZ = m.get_datum(alpha_s);
	superiso_params->mass_Z = m.get_datum(m_z);
	superiso_params->mass_b = m.get_datum(m_b);
	superiso_params->mass_top_pole = m.get_datum(m_top);
	superiso_params->mass_tau_pole  = m.get_datum(m_tau);

	// minpar

	if (model::mSUGRA == m.get_model_type())
	{
		superiso_params->m0 = m.get_datum(m0);
		superiso_params->m12 = m.get_datum(mhf);
		superiso_params->tan_beta = m.get_datum(tb);
		superiso_params->sign_mu = m.get_datum(sgnmu);
		superiso_params->A0 = m.get_datum(a0);
	}
	else // NUSUGRA: only tb, sgn(mu)
	{
		superiso_params->tan_beta = m.get_datum(tb);
		superiso_params->sign_mu = m.get_datum(sgnmu);
	}

	// extpar (NUSUGRA only)
	
	if (model::NUSUGRA == m.get_model_type())
	{
		superiso_params->Min = m.get_datum(mGUT);

		superiso_params->M1_Min = m.get_datum(m1_X);
		superiso_params->M2_Min = m.get_datum(m2_X);
		superiso_params->M3_Min = m.get_datum(m3_X);

		superiso_params->At_Min = m.get_datum(at_X);
		superiso_params->Ab_Min = m.get_datum(ab_X);
		superiso_params->Atau_Min = m.get_datum(atau_X);

		superiso_params->M2H1_Min = m.get_datum(m_h1sq_X);
		superiso_params->M2H2_Min = m.get_datum(m_h2sq_X);

		superiso_params->MeL_Min = m.get_datum(m_e_l_X);
		superiso_params->MmuL_Min = m.get_datum(m_mu_l_X);
		superiso_params->MtauL_Min = m.get_datum(m_tau_l_X);
		superiso_params->MeR_Min = m.get_datum(m_e_r_X);
		superiso_params->MmuR_Min = m.get_datum(m_mu_r_X);
		superiso_params->MtauR_Min = m.get_datum(m_tau_r_X);

		superiso_params->MqL1_Min = m.get_datum(m_ql1_X);
		superiso_params->MqL2_Min = m.get_datum(m_ql2_X);
		superiso_params->MqL3_Min = m.get_datum(m_ql3_X);
		superiso_params->MuR_Min = m.get_datum(m_u_r_X);
		superiso_params->McR_Min = m.get_datum(m_c_r_X);
		superiso_params->MtR_Min = m.get_datum(m_t_r_X);
		superiso_params->MdR_Min = m.get_datum(m_d_r_X);
		superiso_params->MsR_Min = m.get_datum(m_s_r_X);
		superiso_params->MbR_Min = m.get_datum(m_b_r_X);

	}

	// now model type does not matter

	// mass

	superiso_params->mass_W = m.get_datum(m_w);

	superiso_params->mass_h0 = m.get_datum(m_h0);
	superiso_params->mass_H0 = m.get_datum(m_H0);
	superiso_params->mass_A0 = m.get_datum(m_A0);
	superiso_params->mass_H = m.get_datum(m_Hpm);

	superiso_params->mass_gluino = m.get_datum(m_g);

	superiso_params->mass_neut[1] = m.get_datum(m_o1); // indexing is intentional
	superiso_params->mass_neut[2] = m.get_datum(m_o2);
	superiso_params->mass_cha1 = m.get_datum(m_1pm);
	superiso_params->mass_neut[3] = m.get_datum(m_o3);
	superiso_params->mass_neut[4] = m.get_datum(m_o4);
	superiso_params->mass_cha2 = m.get_datum(m_2pm);

	superiso_params->mass_dnl = m.get_datum(m_d_l);
	superiso_params->mass_upl = m.get_datum(m_u_l);
	superiso_params->mass_stl = m.get_datum(m_s_l);
	superiso_params->mass_chl = m.get_datum(m_c_l);
	superiso_params->mass_b1 = m.get_datum(m_b_1);
	superiso_params->mass_t1 = m.get_datum(m_t_1);

	superiso_params->mass_el = m.get_datum(m_e_l);
	superiso_params->mass_nuel = m.get_datum(m_nue_l);
	superiso_params->mass_mul = m.get_datum(m_mu_l);
	superiso_params->mass_numl = m.get_datum(m_numu_l);
	superiso_params->mass_tau1 = m.get_datum(m_stau_1);
	superiso_params->mass_nutl = m.get_datum(m_nu_tau_l);

	superiso_params->mass_dnr = m.get_datum(m_d_r);
	superiso_params->mass_upr = m.get_datum(m_u_r);
	superiso_params->mass_str = m.get_datum(m_s_r);
	superiso_params->mass_chr = m.get_datum(m_c_r);
	superiso_params->mass_b2 = m.get_datum(m_b_2);
	superiso_params->mass_t2 = m.get_datum(m_t_2);

	superiso_params->mass_er = m.get_datum(m_e_r);
	superiso_params->mass_mur = m.get_datum(m_mu_r);
	superiso_params->mass_tau2 = m.get_datum(m_stau_2);

	// alpha
	
	superiso_params->alpha = m.get_datum(higgs_alpha);

	// nmix
	
	superiso_params->neut_mix[1][1] = matblock(nmix,1,1);
	superiso_params->neut_mix[1][2] = matblock(nmix,1,2);
	superiso_params->neut_mix[1][3] = matblock(nmix,1,3);
	superiso_params->neut_mix[1][4] = matblock(nmix,1,4);
	superiso_params->neut_mix[2][1] = matblock(nmix,2,1);
	superiso_params->neut_mix[2][2] = matblock(nmix,2,2);
	superiso_params->neut_mix[2][3] = matblock(nmix,2,3);
	superiso_params->neut_mix[2][4] = matblock(nmix,2,4);
	superiso_params->neut_mix[3][1] = matblock(nmix,3,1);
	superiso_params->neut_mix[3][2] = matblock(nmix,3,2);
	superiso_params->neut_mix[3][3] = matblock(nmix,3,3);
	superiso_params->neut_mix[3][4] = matblock(nmix,3,4);
	superiso_params->neut_mix[4][1] = matblock(nmix,4,1);
	superiso_params->neut_mix[4][2] = matblock(nmix,4,2);
	superiso_params->neut_mix[4][3] = matblock(nmix,4,3);
	superiso_params->neut_mix[4][4] = matblock(nmix,4,4);

	// umix

	superiso_params->charg_Umix[1][1] = matblock(umix,1,1);
	superiso_params->charg_Umix[1][2] = matblock(umix,1,2);
	superiso_params->charg_Umix[2][1] = matblock(umix,2,1);
	superiso_params->charg_Umix[2][2] = matblock(umix,2,2);

	// vmix

	superiso_params->charg_Vmix[1][1] = matblock(vmix,1,1);
	superiso_params->charg_Vmix[1][2] = matblock(vmix,1,2);
	superiso_params->charg_Vmix[2][1] = matblock(vmix,2,1);
	superiso_params->charg_Vmix[2][2] = matblock(vmix,2,2);

	// stopmix

	superiso_params->stop_mix[1][1] = matblock(stopmix,1,1);
	superiso_params->stop_mix[1][2] = matblock(stopmix,1,2);
	superiso_params->stop_mix[2][1] = matblock(stopmix,2,1);
	superiso_params->stop_mix[2][2] = matblock(stopmix,2,2);

	// sbotmix

	superiso_params->sbot_mix[1][1] = matblock(sbotmix,1,1);
	superiso_params->sbot_mix[1][2] = matblock(sbotmix,1,2);
	superiso_params->sbot_mix[2][1] = matblock(sbotmix,2,1);
	superiso_params->sbot_mix[2][2] = matblock(sbotmix,2,2);

	// staumix

	superiso_params->stau_mix[1][1] = matblock(staumix,1,1);
	superiso_params->stau_mix[1][2] = matblock(staumix,1,2);
	superiso_params->stau_mix[2][1] = matblock(staumix,2,1);
	superiso_params->stau_mix[2][2] = matblock(staumix,2,2);

	// gauge

	superiso_params->GAUGE_Q = m.get_datum(gauge_q);
	superiso_params->gp_Q = m.get_datum(gauge_gp);
	superiso_params->g2_Q = m.get_datum(gauge_g);
	superiso_params->g3_Q = m.get_datum(gauge_g3);

	// yu/yd/ye

	superiso_params->YU_Q = m.get_datum(yu_q);
	superiso_params->YD_Q = m.get_datum(yu_q);
	superiso_params->YE_Q = m.get_datum(yu_q);
	superiso_params->yut[3] = m.get_datum(yt);
	superiso_params->yub[3] = m.get_datum(yb);
	superiso_params->yutau[3] = m.get_datum(ytau);

	// hmix

	superiso_params->HMIX_Q = m.get_datum(hmix_q);
	superiso_params->mu_Q = m.get_datum(hmix_mu);
	superiso_params->tanb_GUT = m.get_datum(hmix_tb);
	superiso_params->Higgs_VEV = m.get_datum(hmix_vev);
	superiso_params->mA2_Q = m.get_datum(hmix_ma2);

	// msoft

	superiso_params->MSOFT_Q = m.get_datum(msoft_q);

	superiso_params->M1_Q = m.get_datum(m1_q);
	superiso_params->M2_Q = m.get_datum(m2_q);
	superiso_params->M3_Q = m.get_datum(m3_q);

	superiso_params->M2H1_Q = m.get_datum(m_h1sq_q);
	superiso_params->M2H2_Q = m.get_datum(m_h2sq_q);

	superiso_params->MeL_Q = m.get_datum(m_e_l_q);
	superiso_params->MmuL_Q = m.get_datum(m_mu_l_q);
	superiso_params->MtauL_Q = m.get_datum(m_tau_l_q);

	superiso_params->MeR_Q = m.get_datum(m_e_r_q);
	superiso_params->MmuR_Q = m.get_datum(m_mu_r_q);
	superiso_params->MtauR_Q = m.get_datum(m_tau_r_q);

	superiso_params->MqL1_Q = m.get_datum(m_ql1_q);
	superiso_params->MqL2_Q = m.get_datum(m_ql2_q);
	superiso_params->MqL3_Q = m.get_datum(m_ql3_q);

	superiso_params->MuR_Q = m.get_datum(m_u_r_q);
	superiso_params->McR_Q = m.get_datum(m_c_r_q);
	superiso_params->MtR_Q = m.get_datum(m_t_r_q);
	superiso_params->MdR_Q = m.get_datum(m_d_r_q);
	superiso_params->MsR_Q = m.get_datum(m_s_r_q);
	superiso_params->MbR_Q = m.get_datum(m_b_r_q);

	// au

	superiso_params->AU_Q = m.get_datum(q_au);
	superiso_params->A_u = m.get_datum(au_q);
	superiso_params->A_c = m.get_datum(ac_q);
	superiso_params->A_t = m.get_datum(at_q);

	// ad

	superiso_params->AD_Q = m.get_datum(q_ad);
	superiso_params->A_d = m.get_datum(ad_q);
	superiso_params->A_s = m.get_datum(as_q);
	superiso_params->A_b = m.get_datum(ab_q);

	// ae

	superiso_params->AE_Q = m.get_datum(q_ae);
	superiso_params->A_e = m.get_datum(ae_q);
	superiso_params->A_mu = m.get_datum(amu_q);
	superiso_params->A_tau = m.get_datum(atau_q);

#ifndef SUPERISO_NO_RELIC

	// decay

	superiso_params->width_h0 = m.get_observable(observable::width_h0);
	superiso_params->width_H0 = m.get_observable(observable::width_H0);
	superiso_params->width_A0 = m.get_observable(observable::width_A0);
	superiso_params->width_H = m.get_observable(observable::width_Hpm);

#endif

	slha_adjust(); // post-processing
}

void superiso_driver::init_params()
{
	Init_param(superiso_params.get());
}

void superiso_driver::slha_adjust()
{
	::slha_adjust(superiso_params.get());
}

double superiso_driver::bsgamma()
{
	double C0w[11],C1w[11],C2w[11],C0b[11],C1b[11],C2b[11];
	
	double mu_W=2.* superiso_params->mass_W;
	double mu_b=superiso_params->mass_b_1S/2.;

	CW_calculator(C0w,C1w,C2w,mu_W,superiso_params.get());
	C_calculator_base1(C0w,C1w,C2w,mu_W,C0b,C1b,C2b,mu_b,superiso_params.get());
	return ::bsgamma(C0b,C1b,C2b,mu_b,mu_W,superiso_params.get());
}

double superiso_driver::bsmumu()
{	
	double C0b[11],C1b[11],C0w[11],C1w[11],C2w[11],Cpb[11];
	double complex CQ0b[3],CQ1b[3],CQpb[3];
	
	double mu_W=2.*superiso_params->mass_W;
	double mu_b=superiso_params->mass_b;
				
	CW_calculator(C0w,C1w,C2w,mu_W,superiso_params.get());
	C_calculator_base2(C0w,C1w,mu_W,C0b,C1b,mu_b,superiso_params.get());
	CQ_calculator(CQ0b,CQ1b,mu_W,mu_b,superiso_params.get());
	Cprime_calculator(Cpb,CQpb,mu_W,mu_b,superiso_params.get());

	return Bsmumu(C0b,C1b,CQ0b,CQ1b,Cpb,CQpb,superiso_params.get(),mu_b);
}

double superiso_driver::btaunu()
{	
	return Btaunu(superiso_params.get());
}

double superiso_driver::gmuon()
{	
	return muon_gm2(superiso_params.get());
}

#ifndef SUPERISO_NO_RELIC
double superiso_driver::relic_density()
{

	double Wefftab[NMAX][2];
	int nlines_Weff;

	relicparam paramrelic;

	Init_relic(Wefftab,&nlines_Weff,superiso_params.get());
	Init_cosmomodel(&paramrelic);

	return ::relic_density(Wefftab,nlines_Weff,superiso_params.get(),&paramrelic);
}
#endif

