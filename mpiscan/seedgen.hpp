

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



#pragma once

#ifndef SEEDGEN_HPP
#define SEEDGEN_HPP

#include <vector>
#include <random>

class seedgen
{
public:
	
	typedef std::random_device::result_type seed_type;

	seedgen(unsigned int _num_seeds);

        seed_type operator()();

#ifndef BOOST_MPI
	seed_type* get();
#endif

private:

	void build_seeds();

	unsigned int num_seeds;
	std::vector<seed_type> seeds;
	std::vector<seed_type>::iterator seed_it;

};

#endif



