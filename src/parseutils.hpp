

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


#ifndef PARSEUTILS_H
#define PARSEUTILS_H

#include <ostream>
#include <vector>
#include <iomanip>

#include "libconstrain/model.hpp"

#define DEFAULT_COL_WIDTH 12

std::ostream& operator<< (std::ostream &o, const std::vector<double> &v);

class row_print
{
public:
	row_print(std::ostream *_o) :o(_o) {}
	void operator() (const std::vector<double> &v) const
	{
		*o << v << std::endl;
	}
private:
	std::ostream *o;
};

class row_printer
{
public:
	row_printer(std::ostream *_o) :o(_o) {}
	void operator() (const double &d) const
	{
		*o << std::setw(DEFAULT_COL_WIDTH) << std::setiosflags(std::ios::left) << d << " ";
	}
private:
	std::ostream *o;
};

#endif


