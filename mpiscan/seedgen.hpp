

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

#include <unordered_set>
#include <random>

class seedgen
{
public:
	
	typedef std::random_device::result_type seed_type;

	seedgen(unsigned int _num_seeds);

        seed_type operator()();

	seed_type* get();

private:

	void build_seeds();

	unsigned int num_seeds;
	std::unordered_set<seed_type> seeds;
	std::unordered_set<seed_type>::iterator seed_it;

};

#endif



