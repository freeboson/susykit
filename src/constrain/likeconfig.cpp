/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - likeconfig.cpp:                                                          -
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
#include <memory>
#include <algorithm>
#include <sstream>
#include <constrain/gaussian_datum.hpp>

const std::string hepstats::likeconfig::comment_chars = "#";

std::istream& operator>>(std::istream &is, hepstats::likedist &dist);

void hepstats::likeconfig::process_stream() {
    std::string conf_line;

    while (std::getline(*conf_stream, conf_line)) {
        auto comment_pos = conf_line.find_first_of(comment_chars);

//      std::cerr << std::endl << "before='" << conf_line << "'" << std::endl;  // debugging
        conf_line = conf_line.substr(0, comment_pos);
//      std::cerr << "after='" << conf_line << "'" << std::endl;                // debugging

        std::istringstream parse(conf_line);

        std::string lookup_type, lookup_code, dist_type;
        double theory_err;
        std::string theory_percent_err;

        if (!(parse >> lookup_type)
            || !(parse >> lookup_code)
            || !(parse >> dist_type)
            || !(parse >> theory_err)
            || !(parse >> theory_percent_err)) {
//          std::cerr << "Rejecting this line!" << std::endl;                   // debugging
            continue;
        }

        model_lookup::model_map type;
        if (lookup_type == "special")
            type = model_lookup::special;
        else if (lookup_type == "output")
            type = model_lookup::output;
        else
            type = model_lookup::slha;

        likedist dist;
        if (dist_type == "lower")
            dist = likedist::lower;
        else if (dist_type == "upper")
            dist = likedist::upper;
        else if (dist_type == "poisson")
            dist = likedist::poisson;
        else if (dist_type == "upper_gaussian")
            dist = likedist::upper_gaussian;
        else if (dist_type == "upper_interpolated")
            dist = likedist::upper_interpolated;
        else if (dist_type == "lower_interpolated")
            dist = likedist::lower_interpolated;
        else
            dist = likedist::gaussian; //default

        double exp_val, exp_err;
        switch (dist) {
            case likedist::lower:
            case likedist::upper:
            case likedist::poisson:
            case likedist::upper_gaussian:
            case likedist::gaussian:
                if (!(parse >> exp_val) || !(parse >> exp_err))
                    continue;
                llhood.add_like_term(std::make_unique<gaussian_datum>
                                             (model_lookup(type, lookup_code),
                                              theory_err,
                                              theory_percent_err,
                                              exp_val,
                                              exp_err));
                break;
            case likedist::

        }

        std::string theory_percent_err;

        std::transform(lookup_type.begin(), lookup_type.end(),
                       lookup_type.begin(), ::tolower);
        std::transform(dist_type.begin(), dist_type.end(), dist_type.begin(),
                       ::tolower);
        std::transform(theory_percent_err.begin(), theory_percent_err.end(),
                       theory_percent_err.begin(), ::tolower);


        llhood.add_like_term(
                std::make_unique<likedatum>(
                        dist,
                        model_lookup(type, lookup_code),
                        exp_val,
                        exp_err,
                        theory_err,
                        (bool) (theory_percent_err == "true")
                )
        );
    }
}

std::istream& operator>>(std::istream &is, hepstats::likedist &dist) {
    std::string desc;
    is >> desc;
    std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);

    if (desc == "lower")
        dist = hepstats::likedist::lower;
    else if (desc == "upper")
        dist = hepstats::likedist::upper;
    else if (desc == "poisson")
        dist = hepstats::likedist::poisson;
    else if (desc == "upper_gaussian")
        dist = hepstats::likedist::upper_gaussian;
    else if (desc == "upper_interpolated")
        dist = hepstats::likedist::upper_interpolated;
    else if (desc == "lower_interpolated")
        dist = hepstats::likedist::lower_interpolated;
    else if (desc == "gaussian")
        dist = hepstats::likedist::gaussian;
    else
        is.setstate(std::ios::failbit);

    return is;
}

