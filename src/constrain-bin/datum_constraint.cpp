/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - datum_constraint.cpp:                                                               -
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


#include "datum_constraint.hpp"

using namespace std;

datum_constraint::datum_constraint(const model_lookup &_ml,
                                   const std::string &cons_line)
        : constraint(), ml(_ml) {
    stringstream cline(cons_line);
    cline >> param;
    process_limit(cline);
}

double datum_constraint::get_value(const model &m) const {
    return ml(m);
}

std::string datum_constraint::get_constraint_type() const {
    stringstream ss;
        switch (ml.get_mode()) {
            case model_lookup::slha:
                ss << "Basic";
                break;
            case model_lookup::output:
                ss << "Output";
                break;
            case model_lookup::special:
                ss << "Special";
                break;
        }

    ss << " constraint ";
    return ss.str();
}

std::string datum_constraint::get_constraint_name() const {
    return param;
}
