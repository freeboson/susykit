

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

#include "libconstrain.h"
#include "sujmicro.h"
#include "qpoint_softsusy_opts.hpp"
#include "sujsoft.hpp"
#include "sujfeyn.hpp"
//#include "sujiso.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc, char** argv)
{
	/// Sets up exception handling
	signal(SIGFPE, FPE_ExceptionHandler);

	softsusy_opts *sugra;
	try {
		sugra = new qpoint_opts(argc,argv);
	} catch (exception &e) {
		if (sugra != nullptr)
			delete sugra;
		return 1;
	}

	model m;

	softsusy_driver softsusy(sugra);
	micromegas_driver micro;
	feynhiggs_driver feynhiggs;
//	superiso_driver superiso;

	try { 
		m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 
	} catch (const string &s) { cerr << "SOFTSUSY exception: " << s << endl; return 1;}


	feynhiggs(&m);
	micro(&m);

	cout << m << endl;

	delete sugra;

	return 0;
}


