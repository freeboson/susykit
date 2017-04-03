/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - datum_constraint.hpp:                                                               -
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
#ifndef SUSYKIT_DATUM_CONSTRAINT_HPP
#define SUSYKIT_DATUM_CONSTRAINT_HPP

#include "constraint.hpp"

class datum_constraint : public constraint {
public:
    datum_constraint(const model_lookup &_ml, const std::string &cons_line);

    double get_value(const model &m) const final;
    std::string get_constraint_type() const final;
    std::string get_constraint_name() const final;

private:
    model_lookup ml;
    std::string param;
};


#endif //SUSYKIT_DATUM_CONSTRAINT_HPP
