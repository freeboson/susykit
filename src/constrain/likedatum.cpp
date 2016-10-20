/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - likedatum.cpp:                                                           -
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


#include "constrain/hepstats.hpp"

double hepstats::likedatum::operator()(const model &m, bool *unlikely) const {
    double pred = lookup(m);

    if (lookup.get_mode() == model_lookup::output
        && lookup.get_code() == susy_dict::observable::btaunu) { // special case
        pred *= br_btaunu_sm; // the btaunu value is SUSY/SM ratio
    }

    double tau = (pred_percent_error) ? pred * pred_error : pred_error;

    return calculate_pull(pred, exp->get_limit(m),
                          tau, exp->get_limit_error(m), unlikely);
}


