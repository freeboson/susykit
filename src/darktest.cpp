#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>
#include <fstream>

#include <stdexcept>

#include "libconstrain.hpp"
#include "micromegas_interface.hpp"
#include "qpoint_softsusy_opts.hpp"
#include "softsusy_interface.hpp"
#include "feynhiggs_interface.hpp"
#include "darksusy_interface.hpp"
#include "superiso_interface.hpp"

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
		if (sugra != nullptr)
			delete sugra;
		return 1;
	}

	model m;

	softsusy_driver softsusy(sugra);
	micromegas_driver micro;
	feynhiggs_driver feynhiggs;
	darksusy_driver darksusy;
	superiso_driver superiso;

	try { 
		m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 
	} catch (const string &s) { cerr << "SOFTSUSY exception: " << s << endl; return 1;}


	feynhiggs(&m);

	cerr << "FeynHiggs: delta(rho) = " << m.get_observable(observable::delta_rho) << endl;
	cerr << "FeynHiggs: Br(b->sg)= " << m.get_observable(observable::bsgnlo) << endl;
	cerr << "FeynHiggs: Br(B0s->mu+mu-)= " << m.get_observable(observable::bsmumu) << endl;
	cerr << "FeynHiggs: (gmu-2)/2= " << m.get_observable(observable::gmuon) << endl;
	cerr << endl;

	micro(&m);


	double micro_proton_SI, micro_proton_SD, micro_neutron_SI, micro_neutron_SD, micro_omegah2, micro_delta_rho;
	double micro_bsgamma, micro_bsmumu, micro_btaunu, micro_gmuon;
	double ds_proton_SI, ds_proton_SD, ds_neutron_SI, ds_neutron_SD, ds_omegah2, ds_bsgamma;
	double superiso_bsgamma, superiso_bsmumu, superiso_btaunu, superiso_gmuon;

	micro_delta_rho = m.get_observable(observable::delta_rho);
	micro_omegah2 = m.get_observable(observable::omega);
	micro_proton_SI = m.get_observable(observable::proton_SI);
	micro_proton_SD = m.get_observable(observable::proton_SD);
	micro_neutron_SI = m.get_observable(observable::neutron_SI);
	micro_neutron_SD = m.get_observable(observable::neutron_SD);
	micro_bsgamma = m.get_observable(observable::bsgnlo);
	micro_bsmumu = m.get_observable(observable::bsmumu);
	micro_btaunu = m.get_observable(observable::btaunu);
	micro_gmuon = m.get_observable(observable::gmuon);

	darksusy(&m);

	ds_omegah2 = m.get_observable(observable::omega);
	ds_proton_SI = m.get_observable(observable::proton_SI);
	ds_proton_SD = m.get_observable(observable::proton_SD);
	ds_neutron_SI = m.get_observable(observable::neutron_SI);
	ds_neutron_SD = m.get_observable(observable::neutron_SD);
	ds_bsgamma = m.get_observable(observable::bsgnlo);

	superiso(&m);

	superiso_bsgamma = m.get_observable(observable::bsgnlo);
	superiso_bsmumu = m.get_observable(observable::bsmumu);
	superiso_btaunu = m.get_observable(observable::btaunu);
	superiso_gmuon = m.get_observable(observable::gmuon);

	cerr << "MicrOMEGAs: delta(rho) = " << micro_delta_rho << endl;
	cerr << "MicrOMEGAs: omega h^2 = " << micro_omegah2 << endl;
	cerr << "MicrOMEGAs: proton_SI = " << 1e-36*micro_proton_SI << endl;
	cerr << "MicrOMEGAs: proton_SD = " << 1e-36*micro_proton_SD << endl;
	cerr << "MicrOMEGAs: neutron_SI = " << 1e-36*micro_neutron_SI << endl;
	cerr << "MicrOMEGAs: neutron_SD = " << 1e-36*micro_neutron_SD << endl;
	cerr << "MicrOMEGAs: Br(b->sg) = " << micro_bsgamma << endl;
	cerr << "MicrOMEGAs: Br(B0s->mu+mu-) = " << micro_bsmumu << endl;
	cerr << "MicrOMEGAs: Br(B->tau nu) = " << micro_btaunu << endl;
	cerr << "MicrOMEGAs: (gmu - 2)/2 = " << micro_gmuon << endl;
	cerr << endl;

	cerr << "DarkSUSY: omega h^2 = " << ds_omegah2 << endl;
	cerr << "DarkSUSY: Br(b->sg) = " << ds_bsgamma << endl;
	cerr << "DarkSUSY: proton_SI = " << 1e-36*ds_proton_SI << endl;
	cerr << "DarkSUSY: proton_SD = " << 1e-36*ds_proton_SD << endl;
	cerr << "DarkSUSY: neutron_SI = " << 1e-36*ds_neutron_SI << endl;
	cerr << "DarkSUSY: neutron_SD = " << 1e-36*ds_neutron_SD << endl;
	cerr << endl;

	cerr << "SuperISO Relic: Br(b->sg) = " << superiso_bsgamma << endl;
	cerr << "SuperISO Relic: Br(B0s->mu+mu-) = " << superiso_bsmumu << endl;
	cerr << "SuperISO Relic: Br(B->tau nu) = " << superiso_btaunu << endl;
	cerr << "SuperISO Relic: (gmu - 2)/2 = " << superiso_gmuon << endl;
	cerr << endl;

	delete sugra;

	return 0;
}


