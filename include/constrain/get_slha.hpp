/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - get_slha.hpp:                                                            -
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

