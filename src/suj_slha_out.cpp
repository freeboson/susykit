
#include <iostream>
#include "mycomplex.h"
#include "def.h"
#include "linalg.h"
#include "lowe.h"
#include "rge.h"
#include "softsusy.h"
#include "softpars.h"
#include "susy.h"
#include "utils.h"
#include "numerics.h"

#include <string>
#include <sstream>

using namespace std;


string suj_slha_out(double mGUT, const DoubleVector &pars, double tb, double sgnMu, MssmSoftsusy r)
{
      ostringstream oss;

      if (pars.displayEnd() != 3 && pars.displayEnd() != 49)
      {
		cerr << "pars.displayEnd() should be 3 or 49!" << endl;
		return "";
      }

      bool nusugra = (pars.displayEnd()==49);

      oss.setf(ios::scientific, ios::floatfield);
      oss.precision(8);

      QedQcd d(r.displayDataSet());

      DoubleMatrix stop(2, 2), sbot(2, 2), stau(2, 2);
      sPhysical s(r.displayPhys());

      stop = (s.mu(1, 3) < s.mu(2, 3)) 
      		? rot2d(s.thetat) 
		: rot2dTwist(s.thetat);

      sbot = (s.md(1, 3) < s.md(2, 3)) 
      		? rot2d(s.thetab) 
		: rot2dTwist(s.thetab);

      stau = (s.me(1, 3) < s.me(2, 3)) 
      		? rot2d(s.thetatau) 
		: rot2dTwist(s.thetatau);

      // this is to have the chargino mass positive
      DoubleMatrix umix(rot2d(s.thetaL)), vmix(rot2d(s.thetaR)); 
      if (s.mch(1) < 0.)
      {
        vmix(1, 1) = -vmix(1, 1);
	vmix(1, 2) = -vmix(1, 2);
      }
      if (s.mch(2) < 0.)
      {
        vmix(2, 1) = -vmix(2, 1);
	vmix(2, 2) = -vmix(2, 2);
      }

		// MODSEL, but not SOFTSUSY STYLE
	if (!nusugra)
		oss << "mSUGRA" << "\t"; // (mSUGRA)
	else
		oss << "NUSUGRA" << "\t"; // (non-universal SUGRA)

      oss
      		<< mGUT << "\t" // not in SLHA, except as a comment

		// SMINPUTS
      		<< 1.0 / d.displayAlpha(ALPHA) << "\t"	// alpha_EM(MZ)
      		<< GMU << "\t"				// G_Fermi
      		<< d.displayAlpha(ALPHAS) << "\t"	// alpha_S(MZ)
      		<< r.displayMz() << "\t"		// MZ
      		<< d.displayMbMb()  << "\t"		// Mb(Mb)
      		<< d.displayPoleMt() << "\t"		// Mt(pole)
      		<< d.displayPoleMtau() << "\t";		// Mtau(pole)

	if (!nusugra)  // mSUGRA MINPAR
	{
		oss 
			// MINPAR
      			<< pars.display(1) << "\t" // m0
      			<< pars.display(2) << "\t" // m12
      			<< tb << "\t"
      			<< sgnMu << "\t"
      			<< pars.display(3) << "\t"; // a0
	}
	else	// general nusugra MINPAR (only tb, sgnMu)
	{
		oss 
			// MINPAR
      			<< tb << "\t"
      			<< sgnMu << "\t";
	}

	if (nusugra)	// EXTPAR (not for nusugra only)
	{
		oss
			<< pars.display(1) << "\t"	// M_1(MX)
			<< pars.display(2) << "\t"	// M_2(MX)
			<< pars.display(3) << "\t"	// M_3(MX)
			<< pars.display(11) << "\t"	// At(MX)
			<< pars.display(12) << "\t"	// Ab(MX)
			<< pars.display(13) << "\t"	// Atau(MX)
			<< pars.display(21) << "\t"	// MH1^2(MX)
			<< pars.display(22) << "\t"	// MH2^2(MX)
			<< pars.display(31) << "\t"	// meL(MX)
		        << pars.display(32) << "\t"	//mmuL(MX)            
		        << pars.display(33) << "\t"	//mtauL(MX)           
			<< pars.display(34) << "\t"	//meR(MX)             
			<< pars.display(35) << "\t"	//mmuR(MX)            
			<< pars.display(36) << "\t"	//mtauR(MX)           
			<< pars.display(41) << "\t"	//mqL1(MX)            
			<< pars.display(42) << "\t"	//mqL2(MX)            
			<< pars.display(43) << "\t"	//mqL3(MX)            
			<< pars.display(44) << "\t"	//muR(MX)             
			<< pars.display(45) << "\t"	//mcR(MX)             
			<< pars.display(46) << "\t"	//mtR(MX)             
			<< pars.display(47) << "\t"	//mdR(MX)             
			<< pars.display(48) << "\t"	//msR(MX)             
			<< pars.display(49) << "\t";	//mbR(MX)   
	}

	oss
		// MASS
		<< r.displayMw() << "\t"	// W mass
		<< s.mh0 << "\t"		// h0 mass
		<< s.mH0 << "\t"		// H0 mass
		<< s.mA0 << "\t"		// A0 mass
		<< s.mHpm << "\t"		// H+ mass
		<< s.mGluino << "\t"		// ~g mass
		<< s.mneut(1) << "\t"		// ~o1 mass
		<< s.mneut(2) << "\t"		// ~o2 mass
		<< s.mch(1) << "\t"		// ~1+ mass
		<< s.mneut(3) << "\t"		// ~o3 mass
		<< s.mneut(4) << "\t"		// ~o4 mass
		<< s.mch(2) << "\t"		// ~2+ mass
		<< s.md(1,1) << "\t"		// ~d_L mass
		<< s.mu(1,1) << "\t"		// ~u_L mass
		<< s.md(1,2) << "\t"		// ~s_L mass
		<< s.mu(1,2) << "\t"		// ~c_L mass
		<< minimum(s.md(1, 3), s.md(2, 3)) << "\t"	// ~b_1 mass
		<< minimum(s.mu(1, 3), s.mu(2, 3)) << "\t"	// ~t_1 mass
		<< s.me(1,1) << "\t"		// ~e_L mass
		<< s.msnu(1) << "\t"		// ~nue_L mass
		<< s.me(1,2) << "\t"		// ~mu_L mass
		<< s.msnu(2) << "\t"		// ~numu_L mass
		<< minimum(s.me(1, 3), s.me(2, 3)) << "\t"	// ~stau_1 mass
		<< s.msnu(3) << "\t"		// ~nu_tau_L mass
		<< s.md(2,1) << "\t"		// ~d_R mass
		<< s.mu(2,1) << "\t"		// ~u_R mass
		<< s.md(2,2) << "\t"		// ~s_R mass
		<< s.mu(2,2) << "\t"		// ~c_R mass
		<< maximum(s.md(1, 3), s.md(2, 3)) << "\t"	// ~b_2 mass
		<< maximum(s.mu(1, 3), s.mu(2, 3)) << "\t"	// ~t_2 mass
		<< s.me(2,1) << "\t"		// ~e_R mass
		<< s.me(2,2) << "\t"		// ~mu_R mass
		<< maximum(s.me(1, 3), s.me(2, 3)) << "\t"	// ~stau_2 mass

		// alpha
      		<< s.thetaH << "\t"

		// nmix
      		<< s.mixNeut(1, 1) << "\t"	// N_{1,1}
      		<< s.mixNeut(1, 2) << "\t"	// N_{1,2}
      		<< s.mixNeut(1, 3) << "\t"	// N_{1,3}
      		<< s.mixNeut(1, 4) << "\t"	// N_{1,4}
      		<< s.mixNeut(2, 1) << "\t"	// N_{2,1}
      		<< s.mixNeut(2, 2) << "\t"	// N_{2,2}
      		<< s.mixNeut(2, 3) << "\t"	// N_{2,3}
      		<< s.mixNeut(2, 4) << "\t"	// N_{2,4}
      		<< s.mixNeut(3, 1) << "\t"	// N_{3,1}
      		<< s.mixNeut(3, 2) << "\t"	// N_{3,2}
      		<< s.mixNeut(3, 3) << "\t"	// N_{3,3}
      		<< s.mixNeut(3, 4) << "\t"	// N_{3,4}
      		<< s.mixNeut(4, 1) << "\t"	// N_{4,1}
      		<< s.mixNeut(4, 2) << "\t"	// N_{4,2}
      		<< s.mixNeut(4, 3) << "\t"	// N_{4,3}
      		<< s.mixNeut(4, 4) << "\t"	// N_{4,4}

		// umix
		<< umix(1, 1) << "\t"	// U_{1,1}
		<< umix(1, 2) << "\t"	// U_{1,2}
		<< umix(2, 1) << "\t"	// U_{2,1}
		<< umix(2, 2) << "\t"	// U_{2,2}

		// vmix
		<< vmix(1, 1) << "\t"	// V_{1,1}
		<< vmix(1, 2) << "\t"	// V_{1,2}
		<< vmix(2, 1) << "\t"	// V_{2,1}
		<< vmix(2, 2) << "\t"	// V_{2,2}

		// stopmix
      		<< stop(1,1) << "\t"	// F_{1,1}
      		<< stop(1,2) << "\t"	// F_{1,2}
      		<< stop(2,1) << "\t"	// F_{2,1}
      		<< stop(2,2) << "\t"	// F_{2,2}

		// sbotmix
      		<< sbot(1,1) << "\t"	// F_{1,1}
      		<< sbot(1,2) << "\t"	// F_{1,2}
      		<< sbot(2,1) << "\t"	// F_{2,1}
      		<< sbot(2,2) << "\t"	// F_{2,2}

		// staumix
      		<< stau(1,1) << "\t"	// F_{1,1}
      		<< stau(1,2) << "\t"	// F_{1,2}
      		<< stau(2,1) << "\t"	// F_{2,1}
      		<< stau(2,2) << "\t";	// F_{2,2}

		r.runto(r.displayMsusy());


      oss	
		// gauge
      		<< r.displayMsusy() << "\t" 			// Q
      		<< r.displayGaugeCoupling(1) * sqrt(0.6) << "\t"  //g'(Q) MSSM DRbar
      		<< r.displayGaugeCoupling(2) << "\t"		//g(Q)
      		<< r.displayGaugeCoupling(3) << "\t"		//g3(Q)

		// yu
      		<< r.displayMsusy() << "\t"			// Q
      		<< r.displayYukawaElement(YU, 3, 3) << "\t" 	//Yt(Q)
      		<< r.displayYukawaElement(YD, 3, 3) << "\t" 	//Yb(Q)
      		<< r.displayYukawaElement(YE, 3, 3) << "\t" 	//Ytau(Q)

		// hmix
      		<< r.displayMsusy() << "\t"	// Q
      		<< r.displaySusyMu() << "\t"	// mu(Q)
      		<< r.displayTanb() << "\t"	// tb(Q)
      		<< r.displayHvev() << "\t"	// higgs vev(Q)
      		<< (r.displayM3Squared() / (sin(atan(r.displayTanb())) * cos(atan(r.displayTanb())))) << "\t" //mA^2(Q)

		// msoft
      		<< r.displayMsusy() << "\t"	// Q
      		<< r.displayGaugino(1) << "\t"	// M_1(Q)
      		<< r.displayGaugino(2) << "\t"	// M_2(Q)
      		<< r.displayGaugino(3) << "\t"	// M_3(Q)
      		<< r.displayMh1Squared() << "\t"	// MH1^2(Q)
      		<< r.displayMh2Squared() << "\t"	// MH2^2(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mLl, 1, 1)) << "\t"	// meL(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mLl, 2, 2)) << "\t"	// mmuL(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mLl, 3, 3)) << "\t"	// mtauL(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mEr, 1, 1)) << "\t"	// meR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mEr, 2, 2)) << "\t"	// mmuR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mEr, 3, 3)) << "\t"	// mtauR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mQl, 1, 1)) << "\t"	// mqL1(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mQl, 2, 2)) << "\t"	// mqL2(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mQl, 3, 3)) << "\t"	// mqL3(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mUr, 1, 1)) << "\t"	// muR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mUr, 2, 2)) << "\t"	// mcR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mUr, 3, 3)) << "\t"	// mtR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mDr, 1, 1)) << "\t"	// mdR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mDr, 2, 2)) << "\t"	// msR(Q)
      		<< ccbSqrt(r.displaySoftMassSquared(mDr, 3, 3)) << "\t"	// mbR(Q)

		// au
      		<< r.displayMsusy() << "\t"	// Q
      		<< r.displaySoftA(UA, 1, 1) << "\t"	// Au(Q)
      		<< r.displaySoftA(UA, 2, 2) << "\t"	// Ac(Q)
      		<< r.displaySoftA(UA, 3, 3) << "\t"	// At(Q)

		// ad
      		<< r.displayMsusy() << "\t"	// Q
      		<< r.displaySoftA(DA, 1, 1) << "\t"	// Ad(Q)
      		<< r.displaySoftA(DA, 2, 2) << "\t"	// As(Q)
      		<< r.displaySoftA(DA, 3, 3) << "\t"	// Ab(Q)
      
		// ae
      		<< r.displayMsusy() << "\t"	// Q
      		<< r.displaySoftA(EA, 1, 1) << "\t"	// Ae(Q)
      		<< r.displaySoftA(EA, 2, 2) << "\t"	// Amu(Q)
      		<< r.displaySoftA(EA, 3, 3);	// Atau(Q)

//	cout << oss.str() << endl; 
	return oss.str();
}



