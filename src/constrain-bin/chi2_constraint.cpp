/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - chi2_constraint.cpp:                                                               -
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


#include "chi2_constraint.hpp"

using namespace std;

chi2_constraint::chi2_constraint(hepstats::loglike &&_loglike,
                                 const std::string &limit)
        :loglike(move(_loglike)), param("[chi^2 = -2ln(like)]") {
    stringstream limit_ss(limit);
    process_limit(limit_ss);
}

double chi2_constraint::get_value(const model &m) const {
    return 2*loglike.get_log_like(m);
}

std::string chi2_constraint::get_constraint_type() const {
    return "Statistical constraint ";
}

std::string chi2_constraint::get_constraint_name() const {
    return param;
}

