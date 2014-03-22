
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

#include "seedgen.hpp"

namespace mpi = boost::mpi;
using namespace std;

int main ()
{
	mpi::environment env;
	mpi::communicator world;

	const auto nprocs = world.size();
	const auto curr_rank = world.rank();

	unique_ptr<mt19937_64> gen_ptr; 
	seedgen::seedtype seed;
	fstream writer;

	{
		sstream ss;
		ss << "scan.rank_" << setw(5) << setfill('0') << curr_rank << ".mdb";
		writer.open(ss.str().c_str(), fstream::out | fstream::app);
	}

	if ( 0 == curr_rank )
	{
		seedgen sg(nprocs);
		seed = sg();

		for (int rank = 1; rank < nprocs; rank++)
		{
			world.isend(rank, 0, sg());
		}

	} else {

		world.recv(0, 0, seed);
		
	}

	gen_ptr.reset(new mt19937_64(seed));

	writer << "My seed: " << seed << endl;

	if (writer.is_open()) writer.close();

	return 0;
	
}


