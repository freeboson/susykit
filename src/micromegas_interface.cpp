
#include "susykit/micromegas_interface.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

#include <stdexcept>

#include "sources/micromegas.h"
#include "MSSM/lib/pmodel.h"

using namespace std;


model micromegas_driver::operator() (model m)
{
	if (model::invalid == m.get_model_type())
	{
		return m;
	}
	pass_micromegas_slha_data(m);
	calc_observables(&m);
	return m;
}

int micromegas_driver::operator() (model *m)
{
	if (model::invalid == m->get_model_type())
	{
		return 1;
	}

	pass_micromegas_slha_data(*m);
	calc_observables(m);

	return 0;
}

void micromegas_driver::calc_observables(model *m)
{
	int err=1;
	
	char wimpName[10];
	err=sortOddParticles(wimpName);
	if(err) { 
		return ;
	}
	
	double vdrho, vgmuon, vbsgnlo, vbsmumu, vbtaunu;
	double SMbsg;

	vdrho = deltarho();
	vgmuon = gmuon();
	vbsgnlo = bsgnlo(&SMbsg);
	vbsmumu = bsmumu();
	vbtaunu = btaunu();
	

	////////////////// Relic Density //////////////////
	
	int fast;
	double Beps;

	if (init == relic_mode_setting)
	{
		fast = 0; // this compiles CalcHEP modules which 
		Beps = 0; // will be used in future calculations
	}
	else
	{
		fast=1;
		Beps=1e-5;
	}

	double Omega,Xf;
	
	Omega=darkOmega(&Xf,fast,Beps);


	////////////////// WIMP Nucleon cross sections //////////////////
	double pA0[2],pA5[2],nA0[2],nA5[2];
	double Nmass=0.939; // nucleon mass
	double SCcoeff;        

	if (0 != nucleonAmplitudes(FeScLoop, pA0,pA5,nA0,nA5))
	{
		// return error_str;
		throw(runtime_error("MicrOMEGAs Error: Could not compute nucleonAmplitudes()"));
	}
	SCcoeff=4/M_PI*3.8937966E8*pow(Nmass*Mcdm/(Nmass+ Mcdm),2.);
	
	double ProtonSI, ProtonSD, NeutronSI, NeutronSD;
	
	ProtonSI = SCcoeff*pA0[0]*pA0[0];
	ProtonSD = 3*SCcoeff*pA5[0]*pA5[0];
	NeutronSI = SCcoeff*nA0[0]*nA0[0];
	NeutronSD = 3*SCcoeff*nA5[0]*nA5[0];
	
	////////////////// Output //////////////////
#if 0
	ostringstream oss;

	oss.setf(ios::scientific);
	oss.precision(8);

	oss	<< "1\t\t" // valid bit
		<< vdrho << "\t"
		<< vgmuon << "\t"
		<< vbsgnlo << "\t"
		<< vbsmumu << "\t"
		<< vbtaunu << "\t"
		<< Omega << "\t"
		<< ProtonSI << "\t"
		<< ProtonSD << "\t"
		<< NeutronSI << "\t"
		<< NeutronSD;
	
	return oss.str();
#else
	m->set_observable(susy_dict::observable::micro_valid_bit, 1.0);


// i hate everything.
#undef gmuon
#undef bsgnlo 
#undef bsmumu
#undef btaunu

	m->set_observable(susy_dict::observable::delta_rho, vdrho);
	m->set_observable(susy_dict::observable::gmuon, vgmuon);
	m->set_observable(susy_dict::observable::bsgnlo, vbsgnlo);
	m->set_observable(susy_dict::observable::bsmumu, vbsmumu);
	m->set_observable(susy_dict::observable::btaunu, vbtaunu);
	m->set_observable(susy_dict::observable::omega, Omega);
	m->set_observable(susy_dict::observable::proton_SI, ProtonSI);
	m->set_observable(susy_dict::observable::proton_SD, ProtonSD);
	m->set_observable(susy_dict::observable::neutron_SI, NeutronSI);
	m->set_observable(susy_dict::observable::neutron_SD, NeutronSD);

#define gmuon gmuon_
#define bsgnlo bsgnlo_
#define bsmumu bsmumu_ 
#define btaunu btaunu_

#endif
}

void micromegas_driver::pass_micromegas_slha_data(const model &m)
{
  assignValW( const_cast<char *>("MH3"), m.get_datum(susy_dict::m_A0));
  assignValW( const_cast<char *>("Mh"), m.get_datum(susy_dict::m_h0));
  assignValW( const_cast<char *>("MHH"), m.get_datum(susy_dict::m_H0));
  assignValW( const_cast<char *>("MHc"), m.get_datum(susy_dict::m_Hpm));
  assignValW( const_cast<char *>("MNE1"), m.get_datum(susy_dict::m_o1));
  assignValW( const_cast<char *>("MNE2"), m.get_datum(susy_dict::m_o2));
  assignValW( const_cast<char *>("MNE3"), m.get_datum(susy_dict::m_o3));
  assignValW( const_cast<char *>("MNE4"), m.get_datum(susy_dict::m_o4));
  assignValW( const_cast<char *>("MC1"), m.get_datum(susy_dict::m_1pm));
  assignValW( const_cast<char *>("MC2"), m.get_datum(susy_dict::m_2pm));
  assignValW( const_cast<char *>("MSG"), m.get_datum(susy_dict::m_g));
  assignValW( const_cast<char *>("MSne"), m.get_datum(susy_dict::m_nue_l));
  assignValW( const_cast<char *>("MSnm"), m.get_datum(susy_dict::m_numu_l));
  assignValW( const_cast<char *>("MSnl"), m.get_datum(susy_dict::m_nu_tau_l));
  assignValW( const_cast<char *>("MSeL"), m.get_datum(susy_dict::m_e_l));
  assignValW( const_cast<char *>("MSeR"), m.get_datum(susy_dict::m_e_r));
  assignValW( const_cast<char *>("MSmL"), m.get_datum(susy_dict::m_mu_l));
  assignValW( const_cast<char *>("MSmR"), m.get_datum(susy_dict::m_mu_r));
  assignValW( const_cast<char *>("MSl1"), m.get_datum(susy_dict::m_stau_1));
  assignValW( const_cast<char *>("MSl2"), m.get_datum(susy_dict::m_stau_2));
  assignValW( const_cast<char *>("MSdL"), m.get_datum(susy_dict::m_d_l));
  assignValW( const_cast<char *>("MSdR"), m.get_datum(susy_dict::m_d_r));
  assignValW( const_cast<char *>("MSuL"), m.get_datum(susy_dict::m_u_l));
  assignValW( const_cast<char *>("MSuR"), m.get_datum(susy_dict::m_u_r));
  assignValW( const_cast<char *>("MSsL"), m.get_datum(susy_dict::m_s_l));
  assignValW( const_cast<char *>("MSsR"), m.get_datum(susy_dict::m_s_r));
  assignValW( const_cast<char *>("MScL"), m.get_datum(susy_dict::m_c_l));
  assignValW( const_cast<char *>("MScR"), m.get_datum(susy_dict::m_c_r));
  assignValW( const_cast<char *>("MSb1"), m.get_datum(susy_dict::m_b_1));
  assignValW( const_cast<char *>("MSb2"), m.get_datum(susy_dict::m_b_2));
  assignValW( const_cast<char *>("MSt1"), m.get_datum(susy_dict::m_t_1));
  assignValW( const_cast<char *>("MSt2"), m.get_datum(susy_dict::m_t_2));

//  Q=sqrt(fabs(findValW("MSt1")*findValW("MSt2")));

  assignValW( const_cast<char *>("Zn11"), m.get_datum(susy_dict::nmix_11));
  assignValW( const_cast<char *>("Zn12"), m.get_datum(susy_dict::nmix_12));
  assignValW( const_cast<char *>("Zn13"), m.get_datum(susy_dict::nmix_13));
  assignValW( const_cast<char *>("Zn14"), m.get_datum(susy_dict::nmix_14));
  assignValW( const_cast<char *>("Zn21"), m.get_datum(susy_dict::nmix_21));
  assignValW( const_cast<char *>("Zn22"), m.get_datum(susy_dict::nmix_22));
  assignValW( const_cast<char *>("Zn23"), m.get_datum(susy_dict::nmix_23));
  assignValW( const_cast<char *>("Zn24"), m.get_datum(susy_dict::nmix_24));
  assignValW( const_cast<char *>("Zn31"), m.get_datum(susy_dict::nmix_31));
  assignValW( const_cast<char *>("Zn32"), m.get_datum(susy_dict::nmix_32));
  assignValW( const_cast<char *>("Zn33"), m.get_datum(susy_dict::nmix_33));
  assignValW( const_cast<char *>("Zn34"), m.get_datum(susy_dict::nmix_34));
  assignValW( const_cast<char *>("Zn41"), m.get_datum(susy_dict::nmix_41));
  assignValW( const_cast<char *>("Zn42"), m.get_datum(susy_dict::nmix_42));
  assignValW( const_cast<char *>("Zn43"), m.get_datum(susy_dict::nmix_43));
  assignValW( const_cast<char *>("Zn44"), m.get_datum(susy_dict::nmix_44));

  assignValW( const_cast<char *>("Zu11"), m.get_datum(susy_dict::umix_11));
  assignValW( const_cast<char *>("Zu12"), m.get_datum(susy_dict::umix_12));
  assignValW( const_cast<char *>("Zu21"), m.get_datum(susy_dict::umix_21));
  assignValW( const_cast<char *>("Zu22"), m.get_datum(susy_dict::umix_22));

  assignValW( const_cast<char *>("Zv11"), m.get_datum(susy_dict::vmix_11));
  assignValW( const_cast<char *>("Zv12"), m.get_datum(susy_dict::vmix_12));
  assignValW( const_cast<char *>("Zv21"), m.get_datum(susy_dict::vmix_21));
  assignValW( const_cast<char *>("Zv22"), m.get_datum(susy_dict::vmix_22));

  assignValW( const_cast<char *>("Zb11"),  m.get_datum(susy_dict::sbotmix_11));
  assignValW( const_cast<char *>("Zb12"),  m.get_datum(susy_dict::sbotmix_12));
  #if 0
    assignValW( const_cast<char *>("Zb21"),  m.get_datum(susy_dict::sbotmix_21));
    assignValW( const_cast<char *>("Zb22"),  m.get_datum(susy_dict::sbotmix_22));
  #else
    assignValW( const_cast<char *>("Zb21"),  -m.get_datum(susy_dict::sbotmix_12));
    assignValW( const_cast<char *>("Zb22"),  m.get_datum(susy_dict::sbotmix_11));
  #endif

  assignValW( const_cast<char *>("Zt11"),  m.get_datum(susy_dict::stopmix_11));
  assignValW( const_cast<char *>("Zt12"),  m.get_datum(susy_dict::stopmix_12));
  #if 0
    assignValW( const_cast<char *>("Zt21"),  m.get_datum(susy_dict::stopmix_21));
    assignValW( const_cast<char *>("Zt22"),  m.get_datum(susy_dict::stopmix_22));
  #else
    assignValW( const_cast<char *>("Zt21"),  -m.get_datum(susy_dict::stopmix_12));
    assignValW( const_cast<char *>("Zt22"),  m.get_datum(susy_dict::stopmix_11));
  #endif

  assignValW( const_cast<char *>("Zl11"),  m.get_datum(susy_dict::staumix_11));
  assignValW( const_cast<char *>("Zl12"),  m.get_datum(susy_dict::staumix_12));
  #if 0
    assignValW( const_cast<char *>("Zl21"),  m.get_datum(susy_dict::staumix_21));
    assignValW( const_cast<char *>("Zl22"),  m.get_datum(susy_dict::staumix_22));
  #else
    assignValW( const_cast<char *>("Zl21"),  -m.get_datum(susy_dict::staumix_12));
    assignValW( const_cast<char *>("Zl22"),  m.get_datum(susy_dict::staumix_11));
  #endif

  assignValW( const_cast<char *>("alpha"),  m.get_datum(susy_dict::higgs_alpha));

  assignValW( const_cast<char *>("tb"),  m.get_datum(susy_dict::tb));

  assignValW( const_cast<char *>("mu"),  m.get_datum(susy_dict::hmix_mu));

  assignValW( const_cast<char *>("Am"),  m.get_datum(susy_dict::amu_q));
  assignValW( const_cast<char *>("Al"),  m.get_datum(susy_dict::atau_q));
  assignValW( const_cast<char *>("Ab"),  m.get_datum(susy_dict::ab_q));
  assignValW( const_cast<char *>("Ad"),  m.get_datum(susy_dict::as_q));
  assignValW( const_cast<char *>("At"),  m.get_datum(susy_dict::at_q));
  assignValW( const_cast<char *>("Au"),  m.get_datum(susy_dict::au_q));

  if ( (m.get_datum(susy_dict::m1_q) > 0) && (m.get_datum(susy_dict::m2_q) > 0))
  {
    assignValW( const_cast<char *>("MG1"),  m.get_datum(susy_dict::m1_q));
    assignValW( const_cast<char *>("MG2"),  m.get_datum(susy_dict::m2_q));
  }
  else  // this is an adaptation of CheckNCSector() in rdLesH.o in aLib.a
  {
    double mg1,mg2;
    mg1 = m.get_datum(susy_dict::nmix_11)*m.get_datum(susy_dict::m_o1)*m.get_datum(susy_dict::nmix_11)
      + m.get_datum(susy_dict::nmix_12)*m.get_datum(susy_dict::m_o2)*m.get_datum(susy_dict::nmix_12)
      + m.get_datum(susy_dict::nmix_13)*m.get_datum(susy_dict::m_o3)*m.get_datum(susy_dict::nmix_13)
      + m.get_datum(susy_dict::nmix_14)*m.get_datum(susy_dict::m_o4)*m.get_datum(susy_dict::nmix_14);

    mg2 = m.get_datum(susy_dict::nmix_21)*m.get_datum(susy_dict::m_o1)*m.get_datum(susy_dict::nmix_21)
      + m.get_datum(susy_dict::nmix_22)*m.get_datum(susy_dict::m_o2)*m.get_datum(susy_dict::nmix_22)
      + m.get_datum(susy_dict::nmix_23)*m.get_datum(susy_dict::m_o3)*m.get_datum(susy_dict::nmix_23)
      + m.get_datum(susy_dict::nmix_24)*m.get_datum(susy_dict::m_o4)*m.get_datum(susy_dict::nmix_24);

    assignValW( const_cast<char *>("MG1"), mg1);
    assignValW( const_cast<char *>("MG2"), mg2);
  }

  assignValW( const_cast<char *>("alfSMZ"),  m.get_datum(susy_dict::alpha_s));
  assignValW( const_cast<char *>("MbMb"),  m.get_datum(susy_dict::m_b));
  assignValW( const_cast<char *>("Mtp"),  m.get_datum(susy_dict::m_top));
  assignValW( const_cast<char *>("Ml"),  m.get_datum(susy_dict::m_tau));

  assignValW( const_cast<char *>("dMb"), deltaMb());
  assignValW( const_cast<char *>("dMs"), deltaMs());
  assignValW( const_cast<char *>("dMd"), deltaMd());    

}


