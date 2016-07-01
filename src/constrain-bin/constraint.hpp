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

#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include <iostream>
#include <string>
#include <sstream>

#include "constrain/model.hpp"
#include "constrain/model_lookup.hpp"

#include "constrain/hepstats.hpp"

class constraint {
public:

    constraint();

    constraint(const model_lookup &_ml, const std::string &cons_line);

    constraint(const hepstats::loglike &_loglike, const std::string &limit);

    int operator()(const model &m) const;

    double get_value(const model &m) const;

    std::string get_constraint() const;

private:
    model_lookup ml;
    hepstats::loglike loglike;
    std::string param;
    double lower, upper;
    bool lset, uset;
    bool xset;
    bool like_constraint;

    void process_limit(std::stringstream &ss);

    int check_model(double value) const;
};

#endif

