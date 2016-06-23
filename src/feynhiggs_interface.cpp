
#include "susykit/feynhiggs_interface.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#include <stdexcept>

//#include "FHRecord.h"
#include "feynhiggs/FHCouplings.h"
#include "feynhiggs/SLHADefs.h"

#if defined(nslhadata)
  #if nslhadata != FH_SLHAData_len
    #error FeynHiggs has changed nslhadata macro
  #endif
#else
  #error FeynHiggs does not have nslhadata macro
#endif

#if 0
#include "src/include/CSLHA.h.in"
#else
extern "C" {
extern void FORTRAN(slhaclear)(COMPLEX *slhadata);

extern void FORTRAN(slhawrite)(INTEGER *error,
  CCOMPLEX *slhadata, const char *filename,
  const int filename_len);

static inline void SLHAWrite(int *error,
  CCOMPLEX *slhadata, const char *filename)
{
  FORTRAN(slhawrite)(error, slhadata,
    filename, strlen(filename));
}


}

#define SlhaData Slhadata //macros in FeynHiggs are meant to be case-insensitive
#define Slhadata(i) slha[i-1]
#endif

// this is a useful macro for the lines of a block
// that have matrix elements listed. just pass it 
// the literal root of the matrix variables, and 
// then two literal integers for the row/col
#define matblock(name,i,j) m->get_datum( name ## _ ## i ## j )

using namespace std;
using namespace susy_dict;

feynhiggs_driver::feynhiggs_driver()
	:mssm_scope(4), 		// full MSSM
	field_renorm(0),		// DR-bar
	tanbeta_renorm(0),		// DR-bar
	higgs_mixing(2),		// only CP-even mixing
	p2_approx(0),			// full propagator pole determination
	loop_level(3),			// 3 loops!! (NEW)
	run_mt(1),			// run mt
	bottom_resum(1),		// resum corrections O(tb^n)
	two_loop_complex_approx(0),	// all corrections
	debug_level(0),		// 10 = all; 0 = none
	fh_error_state(0)
{
//	cerr << "Sujeet, initializing FeynHiggs..." << flush;
	FORTRAN(fhsetflags)(&fh_error_state,
				&mssm_scope,
				&field_renorm,
				&tanbeta_renorm,
				&higgs_mixing,
				&p2_approx,
				&loop_level,
				&run_mt,
				&bottom_resum,
				&two_loop_complex_approx	);
//	cerr << " Done!" << endl;

	FORTRAN(fhsetdebug)(&debug_level);

	// the slha array will be populated using macros defined in SLHADefs.h
	// these macros map to Slhadata(i) which i have defined as a macro above
	// the -1 in the macro definition is to get the C array indexing

	FORTRAN(slhaclear)(slha);

	if (fh_error_state != 0)
	{
		cerr << "Error initializing FeynHiggs: fh_error_state=" << fh_error_state << endl;
	}
}

void feynhiggs_driver::operator() (model *m)
{
#undef invalid // holy crap, FeynHiggs, prefix your macros!
	if (m->get_model_type() == model::invalid)
	{
		return;
	}
#define invalid (-999)
	pass_feynhiggs_slha_data(m);
	calc_observables(m);
	return;
}

void feynhiggs_driver::calc_observables(model *m)
{
	REAL bsgMSSM, bsgSM, deltaMsMSSM, deltaMSSM, bsmumuMSSM, bsmumuSM;
	REAL gm2, Deltarho, MWMSSM, MWSM, SW2MSSM, SW2SM, edmeTh, edmn, edmHg;
	INTEGER ccb;

	REAL MHiggs[4];
	COMPLEX SAeff;
	COMPLEX UHiggs[3*3], ZHiggs[3*3];

	// these names have to match the stupid macros in CFeynHiggs.h.in and FHCouplings.h
	COMPLEX couplings[ncouplings], couplingsms[ncouplingsms];
	REAL gammas[ngammas], gammasms[ngammasms];
	INTEGER fast = 1;

	if (nullptr == m)
	{
		cerr << "Null pointer to model! (bad) " << endl;
		return;
	}

	m->set_observable(susy_dict::observable::fh_valid_bit, 0.0);

	FORTRAN(fhflavour)(&fh_error_state, &bsgMSSM, &bsgSM, &deltaMsMSSM, &deltaMSSM, &bsmumuMSSM, &bsmumuSM);
	if (fh_error_state) throw(runtime_error("FeynHiggs Error: Could not compute fhflavour()"));

	FORTRAN(fhconstraints)(&fh_error_state, &gm2, &Deltarho, &MWMSSM, &MWSM, &SW2MSSM, &SW2SM, &edmeTh, &edmn, &edmHg, &ccb);
	if (fh_error_state) throw(runtime_error("FeynHiggs Error: Could not compute fhconstraints()"));

	FORTRAN(fhhiggscorr)(&fh_error_state, MHiggs, &SAeff, UHiggs, ZHiggs);
	if (fh_error_state) throw(runtime_error("FeynHiggs Error: Could not compute fhhiggscorr()"));

	FORTRAN(fhcouplings)(&fh_error_state, couplings, couplingsms, gammas, gammasms, &fast);
	if (fh_error_state) throw(runtime_error("FeynHiggs Error: Could not compute fhcouplings()"));


#if 0
	cout 	<< endl << endl
		<< "Flavour" << endl
		<< "b->sg:\t"		<< bsgMSSM << endl
		<< "delta Ms:\t"	<< deltaMsMSSM << endl
		<< "bs->mu+mu-:\t"	<< bsmumuMSSM << endl
		<< endl
		<< "Constraints" << endl
		<< "g-2:\t" 		<< gm2 << endl
		<< "Delta rho:\t"	<< Deltarho << endl
		<< "MW:\t"		<< MWMSSM << endl
		<< "sin^2(thetaw):\t"	<< SW2MSSM << endl
		<< "edmeTh:\t"		<< edmeTh << endl
		<< "edmn:\t"		<< edmn << endl
		<< "edmHg:\t"		<< edmHg << endl
		<< "ccb:\t"		<< ccb << endl
		<< endl
		<< "FeynHiggs Higgs" << endl
		<< "sin(alpha):\t"	<< SAeff.re << endl
		<< "mh:\t"		<< MHiggs[0] << endl
		<< "mH:\t"		<< MHiggs[1] << endl
		<< "mA:\t"		<< MHiggs[2] << endl
		<< "mHpm:\t"		<< MHiggs[3] << endl
		<< endl
		<< "SOFTSUSY Higgs" << endl
		<< "sin(alpha):\t"	<< m->get_datum(higgs_alpha) << endl
		<< "mh:\t"		<< m->get_datum(m_h0) << endl
		<< "mH:\t"		<< m->get_datum(m_H0) << endl
		<< "mA:\t"		<< m->get_datum(m_A0) << endl
		<< "mHpm:\t"		<< m->get_datum(m_Hpm) << endl
		<< endl;
#endif

	// FeynHiggs reports nans as 'valid'
	if (
		isfinite(MWMSSM)
		&& isfinite(SW2MSSM)
		&& isfinite(edmeTh)
		&& isfinite(edmn)
		&& isfinite(edmHg)
		&& isfinite(deltaMsMSSM)
		&& isfinite(GammaTot(1))
		&& isfinite(GammaTot(2))
		&& isfinite(GammaTot(3))
		&& isfinite(GammaTot(4))
		&& isfinite(SAeff.re)
		&& isfinite(MHiggs[0])
		&& isfinite(MHiggs[1])
		&& isfinite(MHiggs[2])
		&& isfinite(MHiggs[3])
		&& isfinite(Deltarho)
		&& isfinite(bsgMSSM)
		&& isfinite(bsmumuMSSM)
		&& isfinite(gm2)
					)
	{
		
		m->set_observable(susy_dict::observable::fh_valid_bit, 1.0);
		
		m->set_observable(susy_dict::observable::mw_mssm, MWMSSM);
		m->set_observable(susy_dict::observable::sw2_mssm, SW2MSSM);
		m->set_observable(susy_dict::observable::edmeTh, edmeTh);
		m->set_observable(susy_dict::observable::edmn, edmn);
		m->set_observable(susy_dict::observable::edmHg, edmHg);
		m->set_observable(susy_dict::observable::deltaMs_mssm, deltaMsMSSM);
	
		m->set_observable(susy_dict::observable::width_h0, GammaTot(1));
		m->set_observable(susy_dict::observable::width_H0, GammaTot(2));
		m->set_observable(susy_dict::observable::width_A0, GammaTot(3));
		m->set_observable(susy_dict::observable::width_Hpm, GammaTot(4));
	
		m->set_datum(susy_dict::higgs_alpha, SAeff.re);
		m->set_datum(susy_dict::m_h0, MHiggs[0]);
		m->set_datum(susy_dict::m_H0, MHiggs[1]);
		m->set_datum(susy_dict::m_A0, MHiggs[2]);
		m->set_datum(susy_dict::m_Hpm, MHiggs[3]);
	
		// this is replacing micromegas
		// TODO: PREFIX THESE!!!
		m->set_observable(susy_dict::observable::delta_rho, Deltarho);
		m->set_observable(susy_dict::observable::bsgnlo, bsgMSSM);
		m->set_observable(susy_dict::observable::bsmumu, bsmumuMSSM);
		m->set_observable(susy_dict::observable::gmuon, gm2);
	}
	else
	{
		// TODO: report where !isfinite specifically
		throw(runtime_error("FeynHiggs Error: failed isfinite"));
	}

	return;
}

void feynhiggs_driver::pass_feynhiggs_slha_data(const model *m)
{
#if 0
	// this is now done in the constructor
	COMPLEX slha[nslhadata]; 	// COMPLEX is defined in ftypes.h
					// nslhadata is defined in SLHADefs.h

	// the slha array will be populated using macros defined in SLHADefs.h
	// these macros map to Slhadata(i) which i have defined as a macro above
	// the -1 in the macro definition is to get the C array indexing

#endif

	FORTRAN(slhaclear)(slha);

	// sminputs

	SMInputs_invAlfaMZ.re = m->get_datum(alpha_em_inv);
	SMInputs_GF.re = m->get_datum(g_fermi);
	SMInputs_AlfasMZ.re = m->get_datum(alpha_s);
	SMInputs_MZ.re = m->get_datum(m_z);
	SMInputs_Mb.re = m->get_datum(m_b);
	SMInputs_Mt.re = m->get_datum(m_top);
	SMInputs_Mtau.re = m->get_datum(m_tau);

	// minpar

	if (m->get_model_type() == model::mSUGRA)
	{
		MinPar_M0.re = m->get_datum(m0);
		MinPar_M12.re = m->get_datum(mhf);
		MinPar_A.re = m->get_datum(a0);
	}
	MinPar_TB.re = m->get_datum(tb);
	MinPar_signMUE.re = m->get_datum(sgnmu);

	// extpar
	ExtPar_Q.re = m->get_datum(mGUT); // looks like we always need this

	if (m->get_model_type() != model::mSUGRA)
	{
		ExtPar_Q.re = m->get_datum(hmix_q); // this is the first Q that comes to mind
		ExtPar_M1.re = m->get_datum(m1_X);
		ExtPar_M2.re = m->get_datum(m2_X);
		ExtPar_M3.re = m->get_datum(m3_X);
		ExtPar_At.re = m->get_datum(at_X);
		ExtPar_Ab.re = m->get_datum(ab_X);
		ExtPar_Atau.re = m->get_datum(atau_X);
		ExtPar_MHd2.re = m->get_datum(m_h1sq_X);
		ExtPar_MHu2.re = m->get_datum(m_h2sq_X);
		ExtPar_MSL(1).re = m->get_datum(m_e_l_X); // MSL(i) is L-slepton, gen i
		ExtPar_MSL(2).re = m->get_datum(m_mu_l_X);
		ExtPar_MSL(3).re = m->get_datum(m_tau_l_X);
		ExtPar_MSE(1).re = m->get_datum(m_e_r_X); // MSE(i) is R-slepton, gen i
		ExtPar_MSE(2).re = m->get_datum(m_mu_r_X);
		ExtPar_MSE(3).re = m->get_datum(m_tau_r_X);
		ExtPar_MSQ(1).re = m->get_datum(m_ql1_X); // MSQ(i) is L-squark, gen i
		ExtPar_MSQ(2).re = m->get_datum(m_ql2_X);
		ExtPar_MSQ(3).re = m->get_datum(m_ql3_X);
		ExtPar_MSU(1).re = m->get_datum(m_u_r_X); // MSU(i) is R-up-type squark, gen i
		ExtPar_MSU(2).re = m->get_datum(m_c_r_X);
		ExtPar_MSU(3).re = m->get_datum(m_t_r_X);
		ExtPar_MSD(1).re = m->get_datum(m_d_r_X); // MSU(i) is R-down-type squark, gen i
		ExtPar_MSD(2).re = m->get_datum(m_s_r_X);
		ExtPar_MSD(3).re = m->get_datum(m_b_r_X);
	}

	// mass

	Mass_MW.re = m->get_datum(m_w);
	Mass_Mh0.re = m->get_datum(m_h0);
	Mass_MHH.re = m->get_datum(m_H0);
	Mass_MA0.re = m->get_datum(m_A0);
	Mass_MHp.re = m->get_datum(m_Hpm);
	Mass_MGl.re = m->get_datum(m_g);
	Mass_MNeu(1).re = m->get_datum(m_o1); // MNeu(i) is ith neutralino mass state
	Mass_MNeu(2).re = m->get_datum(m_o2);
	Mass_MCha(1).re = m->get_datum(m_1pm); // MCha(i) is ith chargino mass state
	Mass_MNeu(3).re = m->get_datum(m_o3);
	Mass_MNeu(4).re = m->get_datum(m_o4);
	Mass_MCha(2).re = m->get_datum(m_2pm);
	Mass_MSf(1,4,1).re = m->get_datum(m_d_l); // Mass_MSf(i,j,k) is the sfermion 
	Mass_MSf(1,3,1).re = m->get_datum(m_u_l); // mass matrix. i=1,2 corresponds to 
	Mass_MSf(1,4,2).re = m->get_datum(m_s_l); // mass state 1,2 or L,R w/o mixing
	Mass_MSf(1,3,2).re = m->get_datum(m_c_l); // j is for family: j=1 is sneutrinos
	Mass_MSf(1,4,3).re = m->get_datum(m_b_1); // j=2 is sleptons, j=3 is up-squarks
	Mass_MSf(1,3,3).re = m->get_datum(m_t_1); // j=4 is down-squarks. k is for generation
	Mass_MSf(1,2,1).re = m->get_datum(m_e_l); // thus m_d_l has i=1, j=4, k=1.
	Mass_MSf(1,1,1).re = m->get_datum(m_nue_l);
	Mass_MSf(1,2,2).re = m->get_datum(m_mu_l);
	Mass_MSf(1,1,2).re = m->get_datum(m_numu_l);	// some results:
	Mass_MSf(1,2,3).re = m->get_datum(m_stau_1);	// L-u's are (1,3,k)
	Mass_MSf(1,1,3).re = m->get_datum(m_nu_tau_l);   // L-d's are (1,4,k)
	Mass_MSf(2,4,1).re = m->get_datum(m_d_r);	// L-l's are (1,2,k)
	Mass_MSf(2,3,1).re = m->get_datum(m_u_r);	// nu's are (1,1,k)
	Mass_MSf(2,4,2).re = m->get_datum(m_s_r);
	Mass_MSf(2,3,2).re = m->get_datum(m_c_r);	// R-u's are (2,3,k)
	Mass_MSf(2,4,3).re = m->get_datum(m_b_2);	// R-d's are (2,4,k)
	Mass_MSf(2,3,3).re = m->get_datum(m_t_2);	// R-l's are (2,2,k)
	Mass_MSf(2,2,1).re = m->get_datum(m_e_r);
	Mass_MSf(2,2,2).re = m->get_datum(m_mu_r);
	Mass_MSf(2,2,3).re = m->get_datum(m_stau_2);

	// alpha

	Alpha_Alpha.re = m->get_datum(higgs_alpha);

	// nmix

	NMix_ZNeu(1,1).re = matblock(nmix,1,1);
	NMix_ZNeu(1,2).re = matblock(nmix,1,2);
	NMix_ZNeu(1,3).re = matblock(nmix,1,3);
	NMix_ZNeu(1,4).re = matblock(nmix,1,4);
	NMix_ZNeu(2,1).re = matblock(nmix,2,1);
	NMix_ZNeu(2,2).re = matblock(nmix,2,2);
	NMix_ZNeu(2,3).re = matblock(nmix,2,3);
	NMix_ZNeu(2,4).re = matblock(nmix,2,4);
	NMix_ZNeu(3,1).re = matblock(nmix,3,1);
	NMix_ZNeu(3,2).re = matblock(nmix,3,2);
	NMix_ZNeu(3,3).re = matblock(nmix,3,3);
	NMix_ZNeu(3,4).re = matblock(nmix,3,4);
	NMix_ZNeu(4,1).re = matblock(nmix,4,1);
	NMix_ZNeu(4,2).re = matblock(nmix,4,2);
	NMix_ZNeu(4,3).re = matblock(nmix,4,3);
	NMix_ZNeu(4,4).re = matblock(nmix,4,4);

#if 0 // try it with mixing matrices (other than NMIX) transposed 

	// umix

	UMix_UCha(1,1).re = matblock(umix,1,1);
	UMix_UCha(1,2).re = matblock(umix,1,2);
	UMix_UCha(2,1).re = matblock(umix,2,1);
	UMix_UCha(2,2).re = matblock(umix,2,2);

	// vmix

	VMix_VCha(1,1).re = matblock(vmix,1,1);
	VMix_VCha(1,2).re = matblock(vmix,1,2);
	VMix_VCha(2,1).re = matblock(vmix,2,1);
	VMix_VCha(2,2).re = matblock(vmix,2,2);

	// stopmix

	StopMix_USf(1,1).re = matblock(stopmix,1,1);
	StopMix_USf(1,2).re = matblock(stopmix,1,2);
	StopMix_USf(2,1).re = matblock(stopmix,2,1);
	StopMix_USf(2,2).re = matblock(stopmix,2,2);

	// sbotmix

	SbotMix_USf(1,1).re = matblock(sbotmix,1,1);
	SbotMix_USf(1,2).re = matblock(sbotmix,1,2);
	SbotMix_USf(2,1).re = matblock(sbotmix,2,1);
	SbotMix_USf(2,2).re = matblock(sbotmix,2,2);

	// staumix

	StauMix_USf(1,1).re = matblock(staumix,1,1);
	StauMix_USf(1,2).re = matblock(staumix,1,2);
	StauMix_USf(2,1).re = matblock(staumix,2,1);
	StauMix_USf(2,2).re = matblock(staumix,2,2);

#else

	// umix

	UMix_UCha(1,1).re = matblock(umix,1,1);
	UMix_UCha(1,2).re = matblock(umix,2,1);
	UMix_UCha(2,1).re = matblock(umix,1,2);
	UMix_UCha(2,2).re = matblock(umix,2,2);

	// vmix

	VMix_VCha(1,1).re = matblock(vmix,1,1);
	VMix_VCha(1,2).re = matblock(vmix,2,1);
	VMix_VCha(2,1).re = matblock(vmix,1,2);
	VMix_VCha(2,2).re = matblock(vmix,2,2);

	// stopmix

	StopMix_USf(1,1).re = matblock(stopmix,1,1);
	StopMix_USf(1,2).re = matblock(stopmix,2,1);
	StopMix_USf(2,1).re = matblock(stopmix,1,2);
	StopMix_USf(2,2).re = matblock(stopmix,2,2);

	// sbotmix

	SbotMix_USf(1,1).re = matblock(sbotmix,1,1);
	SbotMix_USf(1,2).re = matblock(sbotmix,2,1);
	SbotMix_USf(2,1).re = matblock(sbotmix,1,2);
	SbotMix_USf(2,2).re = matblock(sbotmix,2,2);

	// staumix

	StauMix_USf(1,1).re = matblock(staumix,1,1);
	StauMix_USf(1,2).re = matblock(staumix,2,1);
	StauMix_USf(2,1).re = matblock(staumix,1,2);
	StauMix_USf(2,2).re = matblock(staumix,2,2);

#endif

	// gauge

	Gauge_Q.re = m->get_datum(gauge_q);
	Gauge_g1.re = m->get_datum(gauge_gp); // this is the order that SOFTSUSY does
	Gauge_g2.re = m->get_datum(gauge_g);
	Gauge_g3.re = m->get_datum(gauge_g3);

	// yukawa blocks

	Yu_Q.re = m->get_datum(yu_q);
	Yu_Yt.re = m->get_datum(yt);
	Yd_Q.re = m->get_datum(yu_q);
	Yd_Yb.re = m->get_datum(yb);
	Ye_Q.re = m->get_datum(yu_q);
	Ye_Ytau.re = m->get_datum(ytau);

	// hmix

	HMix_Q.re = m->get_datum(hmix_q);
	HMix_MUE.re = m->get_datum(hmix_mu);
	HMix_TB.re = m->get_datum(hmix_tb);
	HMix_VEV.re = m->get_datum(hmix_vev);
	HMix_MA02.re = m->get_datum(hmix_ma2);

	// msoft
	
	MSoft_Q.re = m->get_datum(msoft_q);
	MSoft_M1.re = m->get_datum(m1_q);
	MSoft_M2.re = m->get_datum(m2_q);
	MSoft_M3.re = m->get_datum(m3_q);
	MSoft_MHd2.re = m->get_datum(m_h1sq_q);
	MSoft_MHu2.re = m->get_datum(m_h2sq_q);
	MSoft_MSL(1).re = m->get_datum(m_e_l_q);
	MSoft_MSL(2).re = m->get_datum(m_mu_l_q);
	MSoft_MSL(3).re = m->get_datum(m_tau_l_q);
	MSoft_MSE(1).re = m->get_datum(m_e_r_q);
	MSoft_MSE(2).re = m->get_datum(m_mu_r_q);
	MSoft_MSE(3).re = m->get_datum(m_tau_r_q);
	MSoft_MSQ(1).re = m->get_datum(m_ql1_q);
	MSoft_MSQ(2).re = m->get_datum(m_ql2_q);
	MSoft_MSQ(3).re = m->get_datum(m_ql3_q);
	MSoft_MSU(1).re = m->get_datum(m_u_r_q);
	MSoft_MSU(2).re = m->get_datum(m_c_r_q);
	MSoft_MSU(3).re = m->get_datum(m_t_r_q);
	MSoft_MSD(1).re = m->get_datum(m_d_r_q);
	MSoft_MSD(2).re = m->get_datum(m_s_r_q);
	MSoft_MSD(3).re = m->get_datum(m_b_r_q);

	// au

	Au_Q.re = m->get_datum(q_au);
	Au_Af(1,1).re = m->get_datum(au_q);
	Au_Af(2,2).re = m->get_datum(ac_q);
	Au_Af(3,3).re = m->get_datum(at_q);

	// ad

	Ad_Q.re = m->get_datum(q_ad);
	Ad_Af(1,1).re = m->get_datum(ad_q);
	Ad_Af(2,2).re = m->get_datum(as_q);
	Ad_Af(3,3).re = m->get_datum(ab_q);

	// ae

	Ae_Q.re = m->get_datum(q_ae);
	Ae_Af(1,1).re = m->get_datum(ae_q);
	Ae_Af(2,2).re = m->get_datum(amu_q);
	Ae_Af(3,3).re = m->get_datum(atau_q);

#if 0

	CREAL sqrts = 14.0;
	CINTEGER key = 0xff;
	const std::string file = "test.fh";

	FORTRAN(fhsetslha)(&fh_error_state, slha);
	FHOutputSLHA(&fh_error_state, slha, 0xff); // key=0xFF means calculate everything
//	FORTRAN(slhawrite)(&fh_error_state, slha, file, 12);
	FORTRAN(fhoutput)(&fh_error_state, file.c_str(), &key, &sqrts, file.length());

#else
	FORTRAN(fhsetslha)(&fh_error_state, slha);
#endif
	if (fh_error_state != 0)
	{
		cerr << "Error passing SLHA data to FeynHiggs: fh_error_state=" << fh_error_state << endl;
	}

	return;
}



