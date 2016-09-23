/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - parse.hpp:                                                               -
    -                                                                          -
    - This file is part of SusyKit, https://freeboson.org/susykit/.            -
    -                                                                          -
    - SusyKit is free software: you can redistribute it and/or modify it under -
    - the terms of the GNU Lesser General Public License as published by the   -
    - Free Software Foundation, either version 3 of the License, or (at your   -
    - option) any later version.                                               -
    -                                                                          -
    - SusyKit is distributed in the hope that it will be useful, but WITHOUT   -
    - ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    -
    - FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     -
    - License for more details.                                                -
    -                                                                          -
    - You should have received a copy of the GNU Lesser General Public License -
    - along with SusyKit.  If not, see <http://www.gnu.org/licenses/>.         -
    -                                                                          -
    ----------------------------------------------------------------------------

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

class model_parser {
public:
    model parse(const std::string &model_line, bool merged = true) const;

private:
    // no data...
};

class stream_model_parser : model_parser {
public:
    stream_model_parser() : i(NULL), merged(false), count(0) { }

    stream_model_parser(std::istream *_i, bool _merged) : i(_i), merged(_merged), count(0) { }

    model operator()();

    unsigned long long get_count() const { return count; }

private:
    std::istream *i;
    bool merged;
    unsigned long long count;
};

unsigned long long count_lines(const std::string &fn);

//std::istream& operator>> (std::istream &i, const model &m);
std::ostream &operator<<(std::ostream &o, const model &m);

#endif

