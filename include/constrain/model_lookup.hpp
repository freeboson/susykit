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
#ifndef MODEL_LOOKUP_HPP
#define MODEL_LOOKUP_HPP

#include <string>
#include "model.hpp"

typedef double (model::*ModelLookup)(const std::string &) const;

class model_lookup {
public:
    enum model_map {
        unset,
        slha,
        output,
        special
    };

    model_lookup() : mode(unset), action(NULL) { }

    model_lookup(model_map _mode, const std::string &_code);

    model_map get_mode() const { return mode; }

    std::string get_code() const { return code; }

    bool good_mode() const { return (action != NULL || mode == special); }

    double operator()(const model &m) const;

private:
    ModelLookup action;
    model_map mode;
    std::string code;
};

#endif


