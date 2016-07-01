#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>
#include <fstream>

#include <stdexcept>

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/parse.hpp"
#include "constrain/get_slha.hpp"
#include "constrain/special_lookups.hpp"

#include "susykit/softsusy_interface.hpp"
#include "../qpoint/qpoint_softsusy_opts.hpp"

using namespace std;
using namespace susy_dict;

int main(int argc, char **argv) {
    /// Sets up exception handling
    signal(SIGFPE, FPE_ExceptionHandler);

    softsusy_opts *sugra;
    try {
        sugra = new qpoint_opts(argc, argv);
    } catch (exception &e) {
        return 1;
    }

    model m;
    get_slha slha;

    softsusy_driver softsusy(sugra);

    try {
        m = softsusy(); // need to check for displayProblem().test() and neutralino LSP
    } catch (const string &s) {
        cerr << "SOFTSUSY exception: " << s << endl;
        return 1;
    }

    cout << slha(m) << endl;

    delete sugra;

    return 0;
}


