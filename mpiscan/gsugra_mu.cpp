
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <cmath>

#include "seedgen.hpp"
#include "mpiscan.hpp"

#include "libconstrain.hpp"
//#include "feynhiggs_interface.hpp"
//#include "softsusy_interface.hpp"
//#include "darksusy_interface.hpp"
//#include "micromegas_interface.hpp"
//#include "superiso_interface.hpp"

using namespace std;

using ulonglong = unsigned long long int;

class gsugra_mu_builder : public point_builder
{
public:

	gsugra_mu_builder()
	:point_builder(49),
		m0_dist(100, 1000),
		mhft_dist(100, 1000),
		m3_dist(2.0, 4.0), // log-scale, 100 GeV -> 10 TeV
		a0_by_m3_dist(-10, 10),
		tanb_dist (2, 62)
	{
		
	}

	softsusy_opts operator()()
	{
		softsusy_opts sugra;

		double m0 = pow(10.0, m0_dist(gen()));
		double mhft = mhft_dist(gen());
		double m3 = m3_dist(gen());
		double a0 = m3 * a0_by_m3_dist(gen());
		double tanb = tanb_dist(gen());

		// gaugino masses

		pars(1) = mhft;
		pars(2) = mhft;
		pars(3) = m3;

		// trilinear couplings
		pars(11) = a0;
		pars(12) = a0;
		pars(13) = a0;

		// Higgs field squared-masses
		pars(21) = m0*abs(m0);
		pars(22) = m0*abs(m0);

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

//		if (m.get_datum(susy_dict::m_h0) < 120.0
//		 	|| m.get_datum(susy_dict::m_h0) > 132.0)
		if (m.get_datum(susy_dict::m_h0) > 132.0)
			return true;

//		if (m.get_observable(susy_dict::observable::omega) > 0.127)
//			return true;

		return false;
	}

private:

	uniform_real_distribution<> m0_dist;
	uniform_real_distribution<> mhft_dist;
	uniform_real_distribution<> m3_dist;
	uniform_real_distribution<> a0_by_m3_dist;
	uniform_real_distribution<> tanb_dist ;

};

int main (int argc, char **argv)
{

	fstream writer;
	string myid;
	gsugra_mu_builder gsugra_mu;

	myid = gsugra_mu.mpi_setup(&writer);

//	feynhiggs_driver feynhiggs;
//	micromegas_driver micro;
//	superiso_driver superiso;

	cerr << myid << "Starting scan..." << endl;

	ulonglong status_interval = 10ULL;

	for (ulonglong npoints = 1ULL; npoints <= 1000000000ULL ; npoints++)
	{

		if ( 0 == npoints % status_interval )
		{
			cerr << myid << npoints << " evaluated in " 
			     << gsugra_mu.get_time() << " seconds." << endl;

			if ( 0 == npoints % (10ULL*status_interval) )
				status_interval *= 10ULL;
		}

		auto sugra = gsugra_mu();

		softsusy_driver softsusy(&sugra);

		try {

			model m = softsusy();
			//feynhiggs(&m);
			//micro(&m);

			if (gsugra_mu.filter_point(m))
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

