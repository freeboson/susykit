/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - loglike.hpp:                                                             -
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
#ifndef LOGLIKE_HPP
#define LOGLIKE_HPP

#include <list>
#include <memory>
#include "model.hpp"

class hepstats::loglike {
public:

    void add_like_term(std::unique_ptr<likedatum> datum) {
        like_terms.push_back(std::move(datum));
    }

    double get_log_like(const model &m) const;

    unsigned int get_num_like_terms() const { return like_terms.size(); }

private:
    std::list<std::unique_ptr<likedatum> > like_terms;
};

#endif


