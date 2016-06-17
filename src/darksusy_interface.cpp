

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


#include <stdexcept>

#include "susykit/darksusy_interface.hpp"

#include "src/include/ftypes.h"
#include "src/include/RecordIndices.h"
#include "src/include/SLHADefs.h"

using namespace std;
using namespace susy_dict;

extern "C" {

	// this is to access the dsslhacom COMMON block (ewww)
	extern struct {
		COMPLEX slha[nslhadata]; // COMPLEX is defined in ftypes.h
	} dsslhacom_; // this is from include/dsslha.f in DS

	// basic darksusy routines for setting up the SLHA data
	extern void FORTRAN(dsinit)();
	extern void FORTRAN(dsfromslha)();
	extern void FORTRAN(dsprep)();

	// routines from darksusy for computations
	extern REAL FORTRAN(dsrdomega)(INTEGER *omtype, INTEGER *fast, REAL *xf, INTEGER *ierr, INTEGER *iwar, INTEGER *nfc);
	extern void FORTRAN(dsddneunuc)(REAL *sigsip, REAL *sigsin, REAL *sigsdp, REAL *sigsdn);
	extern void FORTRAN(dsbsgammafull)(REAL *bsgamma, INTEGER *include_MSSM_flag);

	// this is from FH
	extern void FORTRAN(slhaclear)(COMPLEX *slhadata);
}

// a modification of the FeynHiggs macros for writing the SLHA "data structure"
#define SlhaData Slhadata //macros in FeynHiggs are meant to be case-insensitive
#define Slhadata(i) dsslhacom_.slha[i-1]

darksusy_driver::darksusy_driver()
{
	FORTRAN(dsinit)();
	FORTRAN(slhaclear)(dsslhacom_.slha);
}

int darksusy_driver::operator() (model* const m)
{
	FORTRAN(slhaclear)(dsslhacom_.slha);
#undef invalid // holy crap, FeynHiggs, prefix your macros!
	if (m->get_model_type() == model::invalid)
	{
		return 1;
	}
#define invalid (-999)

	pass_darksusy_slha_data(m);
	calc_observables(m);

	return 0;
}

int darksusy_driver::calc_observables (model* const m)
{
	// TODO: add error checking from DS
	INTEGER omtype = 1, fast = 0, ierr, iwar, nfc;
	REAL xf, omegah2;
	REAL sigsip, sigsin, sigsdp, sigsdn;

	INTEGER include_MSSM_flag = 1;
	REAL bsgamma;

	omegah2 = FORTRAN(dsrdomega)(&omtype, &fast, &xf, &ierr, &iwar, &nfc);
	FORTRAN(dsddneunuc)(&sigsip, &sigsin, &sigsdp, &sigsdn);
	FORTRAN(dsbsgammafull)(&bsgamma, &include_MSSM_flag);

	m->set_observable(susy_dict::observable::micro_valid_bit, 1.0);

	// TODO: FIX THIS!!!
	// don't use DS for this, use SuperIso
	// m->set_observable(susy_dict::observable::delta_rho, -1.0); // set by FH
	m->set_observable(susy_dict::observable::gmuon, -1.0);
	m->set_observable(susy_dict::observable::bsgnlo, bsgamma);
	m->set_observable(susy_dict::observable::bsmumu, -1.0);
	m->set_observable(susy_dict::observable::btaunu, -1.0);

	m->set_observable(susy_dict::observable::omega, omegah2);
#if 0
	m->set_observable(susy_dict::observable::proton_SI, sigsip);
	m->set_observable(susy_dict::observable::proton_SD, sigsdp);
	m->set_observable(susy_dict::observable::neutron_SI, sigsin);
	m->set_observable(susy_dict::observable::neutron_SD, sigsdn);
#else
	m->set_observable(susy_dict::observable::proton_SI, 1e36*sigsip);
	m->set_observable(susy_dict::observable::proton_SD, 1e36*sigsdp);
	m->set_observable(susy_dict::observable::neutron_SI, 1e36*sigsin);
	m->set_observable(susy_dict::observable::neutron_SD, 1e36*sigsdn);
#endif

	return 0;
}

// this is a useful macro for the lines of a block
// that have matrix elements listed. just pass it 
// the literal root of the matrix variables, and 
// then two literal integers for the row/col
#define matblock(name,i,j) m->get_datum( name ## _ ## i ## j )

void darksusy_driver::pass_darksusy_slha_data(const model* const m)
{
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
		MinPar_A.re = m->get_datum(susy_dict::a0);
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

	FORTRAN(dsfromslha)();
	FORTRAN(dsprep)();

	return;
}


