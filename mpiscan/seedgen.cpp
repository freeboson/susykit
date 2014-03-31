

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
#include <iterator>

seedgen::seedgen(unsigned int _num_seeds)
:num_seeds(_num_seeds)
{
	seeds.reserve(num_seeds);
	build_seeds();
	seed_it = seeds.begin();
}

auto seedgen::operator()() -> seed_type
{
	if (seeds.end() == seed_it)
	{
		seed_it = seeds.begin();
		throw std::out_of_range("Requested too many seeds");
	}
	const auto seed = *seed_it;
	std::advance(seed_it, 1);
	return seed;
}

void seedgen::build_seeds()
{
	std::random_device rd;
	while (seeds.size() < num_seeds)
		seeds.insert(rd());
}


auto seedgen::get() -> seed_type*
{
	if (seeds.end() == seed_it)
	{
		seed_it = seeds.begin();
		throw std::out_of_range("Requested too many seeds");
	}
	seed_type* send_seed = &(*seed_it);
	std::advance(seed_it, 1);
	return send_seed;

}



