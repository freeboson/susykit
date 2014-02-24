
#include "sujsoft.hpp"
#include "softsusy.h" // for the bc's

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;


softsusy_driver::softsusy_driver(const softsusy_opts * const _sugra)
:sugra(_sugra)
{
	if (nullptr == sugra)
	{
		cerr << "Error: cannot initialize softsusy_driver with a nullptr" << endl;
		// throw something;
		return;
	}

	init_qedqcd();
}

void softsusy_driver::init_qedqcd()
{
	oneset.setAlpha(ALPHAS, sugra->get_alpha_s()); //alpha_em (not inverted)
	oneset.setAlpha(ALPHA, 1.0/sugra->get_alpha_em_inv());
	oneset.setPoleMt(sugra->get_mtop_pole());
	oneset.setMass(mBottom, sugra->get_mbmb());

	oneset.toMz();      ///< Runs SM fermion masses to MZ

}

#if 0
void softsusy_driver::replace_soft_terms(const softsusy_opts * const _sugra)
{
	sugra->set_mgut_guess(_sugra->get_mgut_guess());
	sugra->set_pars(_sugra->get_pars()); //note this also affects get_bcs()
	sugra->set_sgnmu(_sugra->get_sgnmu()); //note this also affects get_bcs()
	sugra->set_tb(_sugra->get_tb()); //note this also affects get_bcs()
}
#endif

model softsusy_driver::operator() (bool gauge_unification)
{
#ifdef OLD_SOFTSUSY // less than 3.3.10 at least
	double mGUT = rge.lowOrg(sugra->get_bcs(),
				  sugra->get_mgut_guess(),
				  sugra->get_pars(),
				  sugra->get_sgnmu(),
				  sugra->get_tb(),
				  oneset,
				  gauge_unification,
				  false );
#else
	rge.lowOrg(sugra->get_bcs(),
		  sugra->get_mgut_guess(),
		  sugra->get_pars(),
		  sugra->get_sgnmu(),
		  sugra->get_tb(),
		  oneset,
		  gauge_unification,
		  false );
#endif

	double mGUT = rge.displayMxBC();

//	cerr << "mSUGRA: mGUT=" << mGUT << " pars(1)=" << sugra->get_pars()(1) << " pars(2)=" << sugra->get_pars()(2) << " pars(3)=" << sugra->get_pars()(3) << " tb=" << sugra->get_tb() << endl;

	if (rge.displayProblem().testSeriousProblem())
	{
		ostringstream oss;
		oss << "SoftSUSY Error: " << rge.displayProblem();
		throw(oss.str());
	}

	return slha_to_model(mGUT);
}

model softsusy_driver::slha_to_model(const double &mGUT)
{
      ostringstream oss;

      oss.setf(ios::scientific, ios::floatfield);
      oss.precision(8);

      QedQcd d(rge.displayDataSet());

      DoubleMatrix stop(2, 2), sbot(2, 2), stau(2, 2);
      sPhysical s(rge.displayPhys());

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

	bool nusugra = sugra->is_nusugra();

		// MODSEL, but not SOFTSUSY STYLE
	if (!nusugra)
		oss << "mSUGRA" << "\t"; // (mSUGRA)
	else
		oss << "NUSUGRA" << "\t"; // (non-universal SUGRA)

	DoubleVector pars = sugra->get_pars();
	double tb = sugra->get_tb();
	int sgnmu = sugra->get_sgnmu();

      oss
      		<< mGUT << "\t" // not in SLHA, except as a comment

		// SMINPUTS
      		<< 1.0 / d.displayAlpha(ALPHA) << "\t"	// alpha_EM(MZ)
      		<< GMU << "\t"				// G_Fermi
      		<< d.displayAlpha(ALPHAS) << "\t"	// alpha_S(MZ)
      		<< rge.displayMz() << "\t"		// MZ
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
      			<< sgnmu << "\t"
      			<< pars.display(3) << "\t"; // a0
	}
	else	// general nusugra MINPAR (only tb, sgnmu)
	{
		oss 
			// MINPAR
      			<< tb << "\t"
      			<< sgnmu << "\t";
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

	/* 
	 * There is a version dependence here with SOFTSUSY. Basically, the way
	 * masses were stored in sPhysical objects has changed, particularly in 
	 * the Higgs sector. I belive the change is only in >3.4.x but I may be
	 * wrong here. So, we need to check the version of SOFTSUSY and then 
	 * obtain the appropriate data for the the Higgs masses.
	 */


	oss
		// MASS
		<< rge.displayMw() << "\t"	// W mass
#if defined(SOFTSUSY_POST34)
		<< s.mh0(1) << "\t"		// h0 mass
		<< s.mh0(2) << "\t"		// H0 mass
		<< s.mA0(1) << "\t"		// A0 mass
		<< s.mHpm << "\t"		// H+ mass
#else
		<< s.mh0 << "\t"		// h0 mass
		<< s.mH0 << "\t"		// H0 mass
		<< s.mA0 << "\t"		// A0 mass
		<< s.mHpm << "\t"		// H+ mass
#endif
		<< s.mGluino << "\t"		// ~g mass
		<< s.mneut(1) << "\t"		// ~o1 mass
		<< s.mneut(2) << "\t"		// ~o2 mass
		<< abs(s.mch(1)) << "\t"	// ~1+ mass
		<< s.mneut(3) << "\t"		// ~o3 mass
		<< s.mneut(4) << "\t"		// ~o4 mass
		<< abs(s.mch(2)) << "\t"	// ~2+ mass
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

		rge.runto(rge.displayMsusy());


      oss	
		// gauge
      		<< rge.displayMsusy() << "\t" 			// Q
      		<< rge.displayGaugeCoupling(1) * sqrt(0.6) << "\t"  //g'(Q) MSSM DRbar
      		<< rge.displayGaugeCoupling(2) << "\t"		//g(Q)
      		<< rge.displayGaugeCoupling(3) << "\t"		//g3(Q)

		// yu
      		<< rge.displayMsusy() << "\t"			// Q
      		<< rge.displayYukawaElement(YU, 3, 3) << "\t" 	//Yt(Q)
      		<< rge.displayYukawaElement(YD, 3, 3) << "\t" 	//Yb(Q)
      		<< rge.displayYukawaElement(YE, 3, 3) << "\t" 	//Ytau(Q)

		// hmix
      		<< rge.displayMsusy() << "\t"	// Q
      		<< rge.displaySusyMu() << "\t"	// mu(Q)
      		<< rge.displayTanb() << "\t"	// tb(Q)
      		<< rge.displayHvev() << "\t"	// higgs vev(Q)
      		<< (rge.displayM3Squared() / (sin(atan(rge.displayTanb())) * cos(atan(rge.displayTanb())))) << "\t" //mA^2(Q)

		// msoft
      		<< rge.displayMsusy() << "\t"	// Q
      		<< rge.displayGaugino(1) << "\t"	// M_1(Q)
      		<< rge.displayGaugino(2) << "\t"	// M_2(Q)
      		<< rge.displayGaugino(3) << "\t"	// M_3(Q)
      		<< rge.displayMh1Squared() << "\t"	// MH1^2(Q)
      		<< rge.displayMh2Squared() << "\t"	// MH2^2(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mLl, 1, 1)) << "\t"	// meL(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mLl, 2, 2)) << "\t"	// mmuL(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mLl, 3, 3)) << "\t"	// mtauL(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mEr, 1, 1)) << "\t"	// meR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mEr, 2, 2)) << "\t"	// mmuR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mEr, 3, 3)) << "\t"	// mtauR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mQl, 1, 1)) << "\t"	// mqL1(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mQl, 2, 2)) << "\t"	// mqL2(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mQl, 3, 3)) << "\t"	// mqL3(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mUr, 1, 1)) << "\t"	// muR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mUr, 2, 2)) << "\t"	// mcR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mUr, 3, 3)) << "\t"	// mtR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mDr, 1, 1)) << "\t"	// mdR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mDr, 2, 2)) << "\t"	// msR(Q)
      		<< ccbSqrt(rge.displaySoftMassSquared(mDr, 3, 3)) << "\t"	// mbR(Q)

		// au
      		<< rge.displayMsusy() << "\t"	// Q
      		<< rge.displaySoftA(UA, 1, 1) << "\t"	// Au(Q)
      		<< rge.displaySoftA(UA, 2, 2) << "\t"	// Ac(Q)
      		<< rge.displaySoftA(UA, 3, 3) << "\t"	// At(Q)

		// ad
      		<< rge.displayMsusy() << "\t"	// Q
      		<< rge.displaySoftA(DA, 1, 1) << "\t"	// Ad(Q)
      		<< rge.displaySoftA(DA, 2, 2) << "\t"	// As(Q)
      		<< rge.displaySoftA(DA, 3, 3) << "\t"	// Ab(Q)
      
		// ae
      		<< rge.displayMsusy() << "\t"	// Q
      		<< rge.displaySoftA(EA, 1, 1) << "\t"	// Ae(Q)
      		<< rge.displaySoftA(EA, 2, 2) << "\t"	// Amu(Q)
      		<< rge.displaySoftA(EA, 3, 3);	// Atau(Q)

//	cout << oss.str() << endl;

	model_parser mp;
	return mp.parse(oss.str(),false);
}

