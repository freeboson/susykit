

/*

   **************************************************************
   *                                                            *
   * SOFTSUSY Large Mu Scanner                                  *
   * Sujeet Akula                                               *
   * sujeet@freeboson.org                                       *
   *                                                            *
   * To compile:                                                *
   *                                                            *
   *                                                            *
   * To run:                                                    *
   *                                                            *
   *                                                            *
   **************************************************************

*/

#include <iostream>

#include "libconstrain.hpp"

#include "softsusy_interface.hpp"
#include "micromegas_interface.hpp"
#include "feynhiggs_interface.hpp"

#include <string>

using namespace std;


int main(int argc, char** argv)
{

	softsusy_opts point;

	const double m0 = 6000.0, mhf = 200.0, a0 = -12000.0, tb = 25.0;

	DoubleVector pars(3);

	pars(1) = m0;
	pars(2) = mhf;
	pars(3) = a0;

	point.set_pars(pars);
	point.set_tb(tb);


	model m;

	softsusy_driver softsusy(&point);
	micromegas_driver micro;
	feynhiggs_driver feynhiggs;

	try { 
		m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 
	} catch (const string &s) { cerr << "SOFTSUSY exception: " << s << endl; return 1;}

	feynhiggs(&m);
	micro(&m);

	if (model::invalid == m.get_model_type() || model::obs_invalid == m.get_observable_data_type())
	{
		cerr << "Problem with obervables for test point! Please change it!" << endl;
//		return 4;
	}

	cout << "\\Omega_\\chi h^2 = " << m.get_observable(susy_dict::observable::omega) << endl;

	return 0;
}


