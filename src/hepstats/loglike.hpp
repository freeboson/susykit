

#pragma once

#ifndef LOGLIKE_H
#define LOGLIKE_H

#include <list>
#include "model.h"

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


