
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <memory>
#include <sys/time.h>

#ifdef BOOST_MPI

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

namespace mpi = boost::mpi;

#else

#include <mpi.h>

#endif

#include "seedgen.hpp"
#include "mpiscan.hpp"

using namespace std;

using ulonglong = unsigned long long int;

point_builder::point_builder(int pars_end)
:pars(pars_end)
{
	time = curr_time();
}

long double point_builder::curr_time() const
{
	timeval tv;
	gettimeofday(&tv, NULL);
	ulonglong micro_seconds = 
		static_cast<ulonglong>(tv.tv_sec) * 1000000ULL
		+ static_cast<ulonglong>(tv.tv_usec) ;
	return (micro_seconds / 1e6L);
}

string point_builder::mpi_setup(fstream *writer)
{
#ifdef BOOST_MPI
	mpi::environment env;
	mpi::communicator world;

	const auto nprocs = world.size();
	const auto curr_rank = world.rank();
#else

	int argc = 0;

	MPI_Init(&argc, NULL);

	int nprocs, curr_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

#endif

	seedgen::seed_type seed;

	string myid;

	{
		stringstream ss;
		ss << "scan.rank_" << setw(5) << setfill('0') << curr_rank+1 << ".mdb";
		writer->open(ss.str().c_str(), fstream::out | fstream::app);
		
		ss.str("");
		ss << "[Slave #" << curr_rank << "] ";
		myid = ss.str();
	}

	if ( 0 == curr_rank )
	{
		myid = "[Master] ";

		seedgen sg(nprocs);
		seed = sg();

		cerr << myid << "Setting seed to " << seed << endl;

#ifdef BOOST_MPI
		for (int rank = 1; rank < nprocs; rank++)
		{
			cerr << myid << "Sending seed to " << rank << endl;
			world.isend(rank, 0, sg());

		}
#else
		MPI_Request dummy;
		for (int rank = 1; rank < nprocs; rank++)
		{
			cerr << myid << "Sending seed to " << rank << endl;
			MPI_Isend(sg.get(), 1, MPI_UNSIGNED, rank, 0, MPI_COMM_WORLD, &dummy);
		}
#endif

	} else {
		
#ifdef BOOST_MPI
		world.recv(0, 0, seed);
#else

		MPI_Status status;
		MPI_Recv(&seed, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, &status);
#endif
		cerr << myid << "received seed " << seed << endl;
	}

	gen_ptr.reset(new mt19937_64(seed));

	cerr << myid << "MPI setup complete..." << endl;

	return myid;

}

point_builder::~point_builder()
{
#ifndef BOOST_MPI
	MPI_Finalize();
#endif
}


