

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
#ifndef SPECIAL_LOOKUPS_HPP
#define SPECIAL_LOOKUPS_HPP

#include <vector>
#include <algorithm>

#include "model.hpp"

class calc_perms
{
public:
	// ctors
	calc_perms(std::vector<model>::const_iterator _first, std::vector<model>::const_iterator _last);
	// work horse...
	int operator() (int n) const;
private:
	std::vector<model>::const_iterator first, last;
};

class get_lsq
{
public:
	double operator() (const model &m) const;
};

class get_lsl
{
public:
	double operator() (const model &m) const;
};

class get_RGG_approx
{
public:
	double operator() (const model &m) const;
};

#endif


