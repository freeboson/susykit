

/*

    ****************************************************************************
    *                                                                          *
    *                                                                          *
    * Sujeet Akula                                                             *
    * sujeet@freeboson.org                                                     *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    *                                                                          *
    ****************************************************************************

*/


#include "seedgen.hpp"

#include <stdexcept>
#include <unordered_set>
#include <iterator>
#include <algorithm>

using namespace std;

seedgen::seedgen(unsigned int _num_seeds)
:seeds(_num_seeds),num_seeds(_num_seeds)
{
	build_seeds();
	seed_it = begin(seeds);
}

auto seedgen::operator()() -> seed_type
{
	if (seeds.end() == seed_it)
	{
		seed_it = begin(seeds);
		throw out_of_range("Requested too many seeds");
	}
	const auto seed = *seed_it;
	advance(seed_it, 1);
	return seed;
}

void seedgen::build_seeds()
{
	// use u_set just to manage uniqueness
	unordered_set<seed_type> temp_seeds;
	temp_seeds.reserve(num_seeds);

	random_device rd;
	while (temp_seeds.size() < num_seeds)
		temp_seeds.insert(rd());
	
	// keep results in vector for easy 
	// access when making MPI calls
	copy(begin(temp_seeds), end(temp_seeds), begin(seeds));
}

#ifndef BOOST_MPI
auto seedgen::get() -> seed_type*
{
	if (seeds.end() == seed_it)
	{
		seed_it = begin(seeds);
		throw out_of_range("Requested too many seeds");
	}
	seed_type* send_seed = &seeds[seed_it - begin(seeds)];
	advance(seed_it, 1);
	return send_seed;

}
#endif


