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
#ifndef GET_SLHA_HPP
#define GET_SLHA_HPP

#include <string>

#include "model.hpp"

class get_slha {
public:
    std::string operator()(const model &m) const;

private:
    std::string sp_modsel(const model *m) const;
    std::string sminputs(const model *m) const;
    std::string minpar(const model *m) const;
    std::string extpar(const model *m) const;
    std::string mass(const model *m) const;
    std::string alpha(const model *m) const;
    std::string nmix(const model *m) const;
    std::string umix(const model *m) const;
    std::string vmix(const model *m) const;
    std::string stopmix(const model *m) const;
    std::string sbotmix(const model *m) const;
    std::string staumix(const model *m) const;
    std::string gauge(const model *m) const;
    std::string yukawa(const model *m) const;
    std::string hmix(const model *m) const;
    std::string msoft(const model *m) const;
    std::string au(const model *m) const;
    std::string ad(const model *m) const;
    std::string ae(const model *m) const;
};

#endif

