
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "seedgen.hpp"
#include "mpiscan.hpp"

#include "libconstrain.hpp"
#include "feynhiggs_interface.hpp"
#include "softsusy_interface.hpp"
#include "darksusy_interface.hpp"
#include "micromegas_interface.hpp"
#include "superiso_interface.hpp"

using namespace std;

using ulonglong = unsigned long long int;

class nuhm_builder : public point_builder
{
public:

	nuhm_builder()
	:point_builder(49),
		m0_dist(100, 10000),
		mhf_dist(100, 2000),
		a0_by_m0_dist(-8, 8),
		tanb_dist (2, 62),
		delta_mh1_dist(-0.9, 1.0),
		delta_mh2_dist(-0.9, 1.0)
	{
		
	}

	softsusy_opts operator()()
	{
		softsusy_opts sugra;

		double m0 = m0_dist(gen());
		double mhf = mhf_dist(gen());
		double a0 = m0 * a0_by_m0_dist(gen());
		double tanb = tanb_dist(gen());

		double mh1 = m0*(1.0 + delta_mh1_dist(gen()));
		double mh2 = m0*(1.0 + delta_mh2_dist(gen()));

		// gaugino masses

		pars(1) = mhf;
		pars(2) = mhf;
		pars(3) = mhf;

		// trilinear couplings
		pars(11) = a0;
		pars(12) = a0;
		pars(13) = a0;

		// Higgs field squared-masses
		pars(21) = mh1 * abs(mh1);
		pars(22) = mh2 * abs(mh2);

		// scalar leptons
		for (int slepton = 31; slepton <= 36; slepton++)
			pars(slepton) = m0;

		// scalar quarks
		for (int squark = 41; squark <= 49; squark++)
			pars(squark) = m0;

		sugra.set_pars(pars);
		sugra.set_tb(tanb);

		return sugra;
	}

	bool filter_point(const model &m)
	{
		if (susy_dict::m_o1 != m.get_hierarchy(0))
		{
			return true;
		}

		if (m.get_datum(susy_dict::m_h0) < 120.0
		 	|| m.get_datum(susy_dict::m_h0) > 132.0)
			return true;

		if (m.get_observable(susy_dict::observable::omega) > 0.127)
			return true;

		return false;
	}

private:

	uniform_real_distribution<> m0_dist;
	uniform_real_distribution<> mhf_dist;
	uniform_real_distribution<> a0_by_m0_dist;
	uniform_real_distribution<> tanb_dist;
	uniform_real_distribution<> delta_mh1_dist;
	uniform_real_distribution<> delta_mh2_dist;

};

int main (int argc, char **argv)
{

	fstream writer;
	string myid;
	nuhm_builder nuhm;

	myid = nuhm.mpi_setup(&writer);

	feynhiggs_driver feynhiggs;
	micromegas_driver micro;
//	superiso_driver superiso;

	cerr << myid << "Starting scan..." << endl;

	ulonglong status_interval = 10ULL;

	for (ulonglong npoints = 1ULL; npoints <= 1000000000ULL ; npoints++)
	{

		if ( 0 == npoints % status_interval )
		{
			cerr << myid << npoints << " evaluated in " 
			     << nuhm.get_time() << " seconds." << endl;

			if ( 0 == npoints % (10ULL*status_interval) )
				status_interval *= 10ULL;
		}

		auto sugra = nuhm();

		softsusy_driver softsusy(&sugra);

		try {

			model m = softsusy();
			feynhiggs(&m);
			micro(&m);

			if (nuhm.filter_point(m))
				continue;

			writer << m << endl;

		} catch (string &s) {

//			cerr << myid << s << endl;
			continue;

		} catch ( exception &e ) {

			continue;

		}

	}

	if (writer.is_open()) writer.close();

	return 0;
	
}
