/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - likeconfig.hpp:                                                          -
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
#ifndef LIKECONFIG_HPP
#define LIKECONFIG_HPP

#include <string>
#include <istream>

#include "model.hpp"

class hepstats::likeconfig {
public:
    likeconfig(std::istream *_conf_stream)
    : conf_stream(_conf_stream) {
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


