

/*

   **************************************************************
   *                                                            *
   *                                                            *
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
#include "micromegas_interface.hpp"
#include "qpoint_softsusy_opts.hpp"
#include "softsusy_interface.hpp"
#include "feynhiggs_interface.hpp"
#include "darksusy_interface.hpp"
#include "superiso_interface.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc, char** argv)
{
	/// Sets up exception handling
	signal(SIGFPE, FPE_ExceptionHandler);

	qpoint_opts *sugra;
	try {
		sugra = new qpoint_opts(argc,argv);
	} catch (exception &e) {
		if (sugra != nullptr)
			delete sugra;
		return 1;
	}

	model m;

	get_slha slha;

	softsusy_driver softsusy(sugra);
//	micromegas_driver micro;
	feynhiggs_driver feynhiggs;
	darksusy_driver darksusy;
	superiso_driver superiso;

	try { 
		m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 
	} catch (const string &s) { cerr << "SOFTSUSY exception: " << s << endl; return 1;}


	feynhiggs(&m);
	darksusy(&m);
	superiso(&m);

	cout << m << endl;

	if (sugra->slha())
		cerr << slha(m) << endl;

	delete sugra;

	return 0;
}


