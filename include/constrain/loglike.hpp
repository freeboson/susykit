

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
#ifndef LOGLIKE_HPP
#define LOGLIKE_HPP

#include <list>
#include "model.hpp"

class hepstats::loglike
{
public:

	void add_like_term(const likedatum &datum)
	{
		like_terms.push_back(datum);
	}

	double get_log_like(const model &m) const;
	unsigned int get_num_like_terms() const { return like_terms.size(); }

private:
	std::list<likedatum> like_terms;
};

#endif


