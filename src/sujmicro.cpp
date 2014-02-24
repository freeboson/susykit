
#include "sujmicro.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#include "sources/micromegas.h"
#include "MSSM/lib/pmodel.h"

using namespace std;

static const std::string error_str = "0\t\t0\t\t0\t\t0\t\t0\t\t0\t\t0\t\t0\t\t0\t\t0\t\t0";

model micromegas_driver::operator() (model m)
{
	if (model::invalid == m.get_model_type())
	{
		return m;
	}
	pass_micromegas_slha_data(m);
	m.set_observables(calc_observables(m));
	return m;
}


string micromegas_driver::calc_observables(const model &m)
{
	int err=1;
	
	char wimpName[10];
	err=sortOddParticles(wimpName);
	if(err) { 
		return error_str;
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
		return error_str;
	}
	SCcoeff=4/M_PI*3.8937966E8*pow(Nmass*Mcdm/(Nmass+ Mcdm),2.);
	
	double ProtonSI, ProtonSD, NeutronSI, NeutronSD;
	
	ProtonSI = SCcoeff*pA0[0]*pA0[0];
	ProtonSD = 3*SCcoeff*pA5[0]*pA5[0];
	NeutronSI = SCcoeff*nA0[0]*nA0[0];
	NeutronSD = 3*SCcoeff*nA5[0]*nA5[0];
	
	////////////////// Output //////////////////

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
#if 0
  double Emin=1,SMmev=320;/*Energy cut in GeV and solar potential in MV*/
  double  sigmaV;
  double vcs_gz,vcs_gg;
  char txt[100];
  double SpA[NZ],SpE[NZ],SpP[NZ];
  double FluxA[NZ],FluxE[NZ],FluxP[NZ];
  double SpNe[NZ],SpNm[NZ],SpNl[NZ];  
  double Etest=Mcdm/2;
 
printf("\n==== Indirect detection =======\n");  

  sigmaV=calcSpectrum( 1+2+4,SpA,SpE,SpP,SpNe,SpNm,SpNl ,&err);
    /* Returns sigma*v in cm^3/sec.     SpX - calculated spectra of annihilation.
       Use SpectdNdE(E, SpX) to calculate energy distribution in  1/GeV units.
       
       First parameter 1-includes W/Z polarization
                       2-includes gammas for 2->2+gamma
                       4-print cross sections             
    */
  printf("sigmav=%.2E[cm^3/s]\n",sigmaV); 

  if(SpA)
  { 
     double fi=0.,dfi=M_PI/180.; /* angle of sight and 1/2 of cone angle in [rad] */ 
                                                   /* dfi corresponds to solid angle 1.E-3sr */                                             
     printf("Photon flux  for angle of sight f=%.2f[rad]\n"
     "and spherical region described by cone with angle %.4f[rad]\n",fi,2*dfi);
     gammaFluxTab(fi,dfi, sigmaV, SpA, FluxA);

     sprintf(txt,"Photon flux for angle of sight %.2f[rad] and cone angle %.2f[rad]",fi,2*dfi);
     printf("Photon flux = %.2E[cm^2 s GeV]^{-1} for E=%.1f[GeV]\n",SpectdNdE(Etest, FluxA), Etest);       
     if(loopGamma(&vcs_gz,&vcs_gg)==0)
     {
         printf("Gamma  ray lines:\n");
         printf("E=%.2E[GeV]  vcs(Z,A)= %.2E[cm^3/s], flux=%.2E[cm^2 s]^{-1}\n",Mcdm-91.19*91.19/4/Mcdm,vcs_gz,
                               gammaFlux(fi,dfi,vcs_gz));  
         printf("E=%.2E[GeV]  vcs(A,A)= %.2E[cm^3/s], flux=%.2E[cm^2 s]^{-1}\n",Mcdm,vcs_gg, 
                             2*gammaFlux(fi,dfi,vcs_gg));
     }
  }

  if(SpE)
  { 
    posiFluxTab(Emin, sigmaV, SpE, FluxE);
    if(SMmev>0)  solarModulation(SMmev,0.0005,FluxE,FluxE);    
    printf("Positron flux  =  %.2E[cm^2 sr s GeV]^{-1} for E=%.1f[GeV] \n",
    SpectdNdE(Etest, FluxE),  Etest); 
  }
  
  if(SpP)
  {
    pbarFluxTab(Emin, sigmaV, SpP,  FluxP); 
    
    if(SMmev>0)  solarModulation(SMmev,1,FluxP,FluxP);     
    printf("Antiproton flux  =  %.2E[cm^2 sr s GeV]^{-1} for E=%.1f[GeV] \n",
    SpectdNdE(Etest, FluxP),  Etest);     
  }
#endif 


	
	return oss.str();
}

void micromegas_driver::pass_micromegas_slha_data(const model &m)
{
  assignValW("MH3",m.get_datum(susy_dict::m_A0));
  assignValW("Mh",m.get_datum(susy_dict::m_h0));
  assignValW("MHH",m.get_datum(susy_dict::m_H0));
  assignValW("MHc",m.get_datum(susy_dict::m_Hpm));
  assignValW("MNE1",m.get_datum(susy_dict::m_o1));
  assignValW("MNE2",m.get_datum(susy_dict::m_o2));
  assignValW("MNE3",m.get_datum(susy_dict::m_o3));
  assignValW("MNE4",m.get_datum(susy_dict::m_o4));
  assignValW("MC1",m.get_datum(susy_dict::m_1pm));
  assignValW("MC2",m.get_datum(susy_dict::m_2pm));
  assignValW("MSG",m.get_datum(susy_dict::m_g));
  assignValW("MSne",m.get_datum(susy_dict::m_nue_l));
  assignValW("MSnm",m.get_datum(susy_dict::m_numu_l));
  assignValW("MSnl",m.get_datum(susy_dict::m_nu_tau_l));
  assignValW("MSeL",m.get_datum(susy_dict::m_e_l));
  assignValW("MSeR",m.get_datum(susy_dict::m_e_r));
  assignValW("MSmL",m.get_datum(susy_dict::m_mu_l));
  assignValW("MSmR",m.get_datum(susy_dict::m_mu_r));
  assignValW("MSl1",m.get_datum(susy_dict::m_stau_1));
  assignValW("MSl2",m.get_datum(susy_dict::m_stau_2));
  assignValW("MSdL",m.get_datum(susy_dict::m_d_l));
  assignValW("MSdR",m.get_datum(susy_dict::m_d_r));
  assignValW("MSuL",m.get_datum(susy_dict::m_u_l));
  assignValW("MSuR",m.get_datum(susy_dict::m_u_r));
  assignValW("MSsL",m.get_datum(susy_dict::m_s_l));
  assignValW("MSsR",m.get_datum(susy_dict::m_s_r));
  assignValW("MScL",m.get_datum(susy_dict::m_c_l));
  assignValW("MScR",m.get_datum(susy_dict::m_c_r));
  assignValW("MSb1",m.get_datum(susy_dict::m_b_1));
  assignValW("MSb2",m.get_datum(susy_dict::m_b_2));
  assignValW("MSt1",m.get_datum(susy_dict::m_t_1));
  assignValW("MSt2",m.get_datum(susy_dict::m_t_2));

//  Q=sqrt(fabs(findValW("MSt1")*findValW("MSt2")));

  assignValW("Zn11",m.get_datum(susy_dict::nmix_11));
  assignValW("Zn12",m.get_datum(susy_dict::nmix_12));
  assignValW("Zn13",m.get_datum(susy_dict::nmix_13));
  assignValW("Zn14",m.get_datum(susy_dict::nmix_14));
  assignValW("Zn21",m.get_datum(susy_dict::nmix_21));
  assignValW("Zn22",m.get_datum(susy_dict::nmix_22));
  assignValW("Zn23",m.get_datum(susy_dict::nmix_23));
  assignValW("Zn24",m.get_datum(susy_dict::nmix_24));
  assignValW("Zn31",m.get_datum(susy_dict::nmix_31));
  assignValW("Zn32",m.get_datum(susy_dict::nmix_32));
  assignValW("Zn33",m.get_datum(susy_dict::nmix_33));
  assignValW("Zn34",m.get_datum(susy_dict::nmix_34));
  assignValW("Zn41",m.get_datum(susy_dict::nmix_41));
  assignValW("Zn42",m.get_datum(susy_dict::nmix_42));
  assignValW("Zn43",m.get_datum(susy_dict::nmix_43));
  assignValW("Zn44",m.get_datum(susy_dict::nmix_44));

  assignValW("Zu11",m.get_datum(susy_dict::umix_11));
  assignValW("Zu12",m.get_datum(susy_dict::umix_12));
  assignValW("Zu21",m.get_datum(susy_dict::umix_21));
  assignValW("Zu22",m.get_datum(susy_dict::umix_22));

  assignValW("Zv11",m.get_datum(susy_dict::vmix_11));
  assignValW("Zv12",m.get_datum(susy_dict::vmix_12));
  assignValW("Zv21",m.get_datum(susy_dict::vmix_21));
  assignValW("Zv22",m.get_datum(susy_dict::vmix_22));

  assignValW("Zb11", m.get_datum(susy_dict::sbotmix_11));
  assignValW("Zb12", m.get_datum(susy_dict::sbotmix_12));
  #if 0
    assignValW("Zb21", m.get_datum(susy_dict::sbotmix_21));
    assignValW("Zb22", m.get_datum(susy_dict::sbotmix_22));
  #else
    assignValW("Zb21", -m.get_datum(susy_dict::sbotmix_12));
    assignValW("Zb22", m.get_datum(susy_dict::sbotmix_11));
  #endif

  assignValW("Zt11", m.get_datum(susy_dict::stopmix_11));
  assignValW("Zt12", m.get_datum(susy_dict::stopmix_12));
  #if 0
    assignValW("Zt21", m.get_datum(susy_dict::stopmix_21));
    assignValW("Zt22", m.get_datum(susy_dict::stopmix_22));
  #else
    assignValW("Zt21", -m.get_datum(susy_dict::stopmix_12));
    assignValW("Zt22", m.get_datum(susy_dict::stopmix_11));
  #endif

  assignValW("Zl11", m.get_datum(susy_dict::staumix_11));
  assignValW("Zl12", m.get_datum(susy_dict::staumix_12));
  #if 0
    assignValW("Zl21", m.get_datum(susy_dict::staumix_21));
    assignValW("Zl22", m.get_datum(susy_dict::staumix_22));
  #else
    assignValW("Zl21", -m.get_datum(susy_dict::staumix_12));
    assignValW("Zl22", m.get_datum(susy_dict::staumix_11));
  #endif

  assignValW("alpha", m.get_datum(susy_dict::higgs_alpha));

  assignValW("tb", m.get_datum(susy_dict::tb));

  assignValW("mu", m.get_datum(susy_dict::hmix_mu));

  assignValW("Am", m.get_datum(susy_dict::amu_q));
  assignValW("Al", m.get_datum(susy_dict::atau_q));
  assignValW("Ab", m.get_datum(susy_dict::ab_q));
  assignValW("Ad", m.get_datum(susy_dict::as_q));
  assignValW("At", m.get_datum(susy_dict::at_q));
  assignValW("Au", m.get_datum(susy_dict::au_q));

  if ( (m.get_datum(susy_dict::m1_q) > 0) && (m.get_datum(susy_dict::m2_q) > 0))
  {
    assignValW("MG1", m.get_datum(susy_dict::m1_q));
    assignValW("MG2", m.get_datum(susy_dict::m2_q));
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

    assignValW("MG1",mg1);
    assignValW("MG2",mg2);
  }

  assignValW("alfSMZ", m.get_datum(susy_dict::alpha_s));
  assignValW("MbMb", m.get_datum(susy_dict::m_b));
  assignValW("Mtp", m.get_datum(susy_dict::m_top));
  assignValW("Ml", m.get_datum(susy_dict::m_tau));

  assignValW("dMb",deltaMb());
  assignValW("dMs",deltaMs());
  assignValW("dMd",deltaMd());    
}


