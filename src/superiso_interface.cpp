
#include "superiso_interface.hpp"

#include <complex.h>

using namespace susy_dict;

struct parameters
/* structure containing all the scanned parameters from the SLHA file */
{
	int SM;
	int model; /* CMSSM=1, GMSB=2, AMSB=3 */
	int generator; /* ISAJET=1, SOFTSUSY=3, SPHENO=4, SUSPECT=5, NMSSMTOOLS=6 */
	double Q; /* Qmax ; default = M_EWSB = sqrt(m_stop1*mstop2) */
	double m0,m12,tan_beta,sign_mu,A0; /* CMSSM parameters */
	double Lambda,Mmess,N5,cgrav,m32; /* AMSB, GMSB parameters */
	double mass_Z,mass_W,mass_b,mass_top_pole,mass_tau_pole; /* SM parameters */
	double inv_alpha_em,alphas_MZ,Gfermi,GAUGE_Q; /* SM parameters */
	double charg_Umix[3][3],charg_Vmix[3][3],stop_mix[3][3],sbot_mix[3][3],stau_mix[3][3],neut_mix[6][6],mass_neut[6],alpha; /* mass mixing matrices */
	double Min,M1_Min,M2_Min,M3_Min,At_Min,Ab_Min,Atau_Min,M2H1_Min,M2H2_Min,mu_Min,M2A_Min,tb_Min,mA_Min; /* optional input parameters at scale Min */
	double MeL_Min,MmuL_Min,MtauL_Min,MeR_Min,MmuR_Min,MtauR_Min; /* optional input parameters at scale Min */
	double MqL1_Min,MqL2_Min,MqL3_Min,MuR_Min,McR_Min,MtR_Min,MdR_Min,MsR_Min,MbR_Min; /* optional input parameters at scale Min */
	double N51,N52,N53,M2H1_Q,M2H2_Q; /* optional input parameters (N51...3: GMSB) */
	double mass_d,mass_u,mass_s,mass_c,mass_t,mass_e,mass_nue,mass_mu,mass_num,mass_tau,mass_nut; /* SM masses */
	double mass_gluon,mass_photon,mass_Z0; /* SM masses */
	double mass_h0,mass_H0,mass_A0,mass_H,mass_dnl,mass_upl,mass_stl,mass_chl,mass_b1,mass_t1; /* Higgs & superparticle masses */
	double mass_el,mass_nuel,mass_mul,mass_numl,mass_tau1,mass_nutl,mass_gluino,mass_cha1,mass_cha2; /* superparticle masses */
	double mass_dnr,mass_upr,mass_str,mass_chr,mass_b2,mass_t2,mass_er,mass_mur,mass_tau2; /* superparticle masses */
	double mass_nuer,mass_numr,mass_nutr,mass_graviton,mass_gravitino; /* superparticle masses */
	double gp,g2,gp_Q,g2_Q,g3_Q,YU_Q,yut[4],YD_Q,yub[4],YE_Q,yutau[4]; /* couplings */
	double HMIX_Q,mu_Q,tanb_GUT,Higgs_VEV,mA2_Q,MSOFT_Q,M1_Q,M2_Q,M3_Q; /* parameters at scale Q */
	double MeL_Q,MmuL_Q,MtauL_Q,MeR_Q,MmuR_Q,MtauR_Q,MqL1_Q,MqL2_Q,MqL3_Q,MuR_Q,McR_Q,MtR_Q,MdR_Q,MsR_Q,MbR_Q; /* masses at scale Q */
	double AU_Q,A_u,A_c,A_t,AD_Q,A_d,A_s,A_b,AE_Q,A_e,A_mu,A_tau; /* trilinear couplings */
	
	/* SLHA2 */
	int NMSSM,RV,CPV,FV;
	double mass_nutau2,mass_e2,mass_nue2,mass_mu2,mass_numu2,mass_d2,mass_u2,mass_s2,mass_c2;
	double CKM_lambda,CKM_A,CKM_rhobar,CKM_etabar;
	double PMNS_theta12,PMNS_theta23,PMNS_theta13,PMNS_delta13,PMNS_alpha1,PMNS_alpha2;
	double lambdaNMSSM_Min,kappaNMSSM_Min,AlambdaNMSSM_Min,AkappaNMSSM_Min,lambdaSNMSSM_Min,xiFNMSSM_Min,xiSNMSSM_Min,mupNMSSM_Min,mSp2NMSSM_Min,mS2NMSSM_Min,mass_H03,mass_A02,NMSSMRUN_Q,lambdaNMSSM,kappaNMSSM,AlambdaNMSSM,AkappaNMSSM,lambdaSNMSSM,xiFNMSSM,xiSNMSSM,mupNMSSM,mSp2NMSSM,mS2NMSSM; /* NMSSM parameters */
	double PMNSU_Q,CKM_Q,IMCKM_Q,MSE2_Q,MSU2_Q,MSD2_Q,MSL2_Q,MSQ2_Q,TU_Q,TD_Q,TE_Q;
	double CKM[4][4],IMCKM[4][4]; /* CKM matrix */
	double H0_mix[4][4],A0_mix[4][4]; /* Higgs mixing matrices */
	double sU_mix[7][7],sD_mix[7][7],sE_mix[7][7], sNU_mix[4][4]; /* mixing matrices */
	double sCKM_msq2[4][4],sCKM_msl2[4][4],sCKM_msd2[4][4],sCKM_msu2[4][4],sCKM_mse2[4][4]; /* super CKM matrices */
	double PMNS_U[4][4]; /* PMNS mixing matrices */
	double TU[4][4],TD[4][4],TE[4][4]; /* trilinear couplings */
	
	/* non-SLHA*/
	double mass_b_1S,mass_b_pole,mtmt;
	double Lambda5; /* Lambda QCD */
	
	/* Flavour constants */
	double f_B,f_Bs,f_Ds,f_D,fK_fpi;
	double f_K_par,f_K_perp;
	double m_B,m_Bs,m_Bd,m_pi,m_Ds,m_K,m_Kstar,m_D0,m_D;
	double life_pi,life_K,life_B,life_Bs,life_Bd,life_D,life_Ds;
	double a1par,a2par,a1perp,a2perp;
	double zeta3A,zeta3V,wA10,deltatp,deltatm;
	double lambda_Bp,rho1,lambda2;
	
	/* Decay widths */
	int widthcalc; /* 0=none, 1=hdecay, 2=feynhiggs */
	double width_h0,width_H0,width_A0,width_H,width_Z,width_W,width_top,width_H03,width_A02;
	double width_gluino,width_t1,width_t2,width_b1,width_b2,width_ul,width_ur,width_dl,width_dr;
	double width_cl,width_cr,width_sl,width_sr,width_el,width_er,width_ml,width_mr,width_tau1,width_tau2;
	double width_nuel,width_numl,width_nutaul,width_c1,width_c2,width_o1,width_o2,width_o3,width_o4,width_o5;
			
	/* CKM matrix */
	std::complex<double> Vud,Vus,Vub,Vcd,Vcs,Vcb,Vtd,Vts,Vtb;
	
	/* 2HDM */
	int THDM_model;
	double lambda_u[4][4],lambda_d[4][4],lambda_l[4][4];

	/* NMSSMTools */
	int NMSSMcoll,NMSSMtheory,NMSSMups1S,NMSSMetab1S;
};


superiso_driver::superiso_driver()
:superiso_params(new parameters)
{
	superiso->init_params(superiso_params.get());
}

void superiso_driver::operator() (model *m)
{
	pass_superiso_slha_data(*m);
	calc_observables(m);
}

void superiso_driver::calc_observables(model *m)
{

}


// this is a useful macro for the lines of a block
// that have matrix elements listed. just pass it 
// the literal root of the matrix variables, and 
// then two literal integers for the row/col
#define matblock(name,i,j) m.get_datum( name ## _ ## i ## j )

void superiso_driver::pass_superiso_slha_data(const model &m)
{
	superiso->init_params(superiso_params.get());

	if (model::mSUGRA == m.get_model_type())
		superiso_params->model = 1;
	else if (model::NUSUGRA == m.get_model_type())
		superiso_params->model = 1; // ???
	else
		return;
	
	// sminputs
	
	superiso_params->inv_alpha_em = m.get_datum(alpha_em_inv);
	superiso_params->Gfermi = m.get_datum(g_fermi);
	superiso_params->alphas_MZ = m.get_datum(g_fermi);
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

	// decay

	superiso_params->width_h0 = m.get_observable(observable::width_h0);
	superiso_params->width_H0 = m.get_observable(observable::width_H0);
	superiso_params->width_A0 = m.get_observable(observable::width_A0);
	superiso_params->width_H = m.get_observable(observable::width_Hpm);

	superiso->slha_adjust(superiso_params.get()); // post-processing
}

