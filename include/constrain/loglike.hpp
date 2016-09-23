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


