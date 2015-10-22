#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>
#include <fstream>

#include <stdexcept>

#include "libconstrain/dict.hpp"
#include "libconstrain/model.hpp"
#include "libconstrain/parse.hpp"
#include "libconstrain/get_slha.hpp"
#include "libconstrain/special_lookups.hpp"

#include "libsusykit/softsusy_interface.hpp"
#include "qpoint_softsusy_opts.hpp"

using namespace std;
using namespace susy_dict;

int main(int argc, char** argv)
{
	/// Sets up exception handling
	signal(SIGFPE, FPE_ExceptionHandler);

	softsusy_opts *sugra;
	try {
		sugra = new qpoint_opts(argc,argv);
	} catch (exception &e) {
		return 1;
	}

	model m;

	softsusy_driver softsusy(sugra);

	try { 
		m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 
	} catch (const string &s) { cerr << "SOFTSUSY exception: " << s << endl; return 1;}

	delete sugra;

	return 0;
}


