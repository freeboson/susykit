

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
#ifndef PARSE_HPP
#define PARSE_HPP

#include <istream>
#include <ostream>
#include <functional>
#include <string>
#include <iterator>

#include "model.hpp"

class model_parser
{
public:
	model parse(const std::string &model_line, bool merged=true) const;
private:
	// no data...
};

class stream_model_parser : model_parser
{
public:
	stream_model_parser() :i(NULL),merged(false),count(0) {}
	stream_model_parser(std::istream *_i, bool _merged) :i(_i),merged(_merged),count(0) {}
	model operator() ();
	unsigned long long get_count() const { return count; }
private:
	std::istream *i;
	bool merged;
	unsigned long long count;
};

unsigned long long count_lines (const std::string &fn);

//std::istream& operator>> (std::istream &i, const model &m);
std::ostream& operator<< (std::ostream &o, const model &m);

#endif

