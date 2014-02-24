
#pragma once
#ifndef LIKECONFIG_H
#define LIKECONFIG_H

#include <string>
#include <istream>

#include "libconstrain.h"

class hepstats::likeconfig
{
public:
	likeconfig(std::istream *_conf_stream)
		:conf_stream(_conf_stream)
	{
		process_stream();
	}

	loglike operator()() const { return get_loglike_fn(); }
	loglike get_loglike_fn() const { return llhood; }

private:
	void process_stream();

	std::istream *conf_stream;
	loglike llhood;

	static const std::string comment_chars;
};

#endif


