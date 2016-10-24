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


#include <memory>
#include <algorithm>
#include <sstream>
#include "constrain/hepstats.hpp"

#include "gaussian.hpp"
#include "upper_gaussian.hpp"
#include "smeared_lower_limit.hpp"
#include "smeared_upper_limit.hpp"
#include "simple_datum.hpp"
#include "interpolated_data.hpp"

const std::string hepstats::likeconfig::comment_chars = "#";

std::istream &operator>>(std::istream &is, hepstats::likedist &dist);

std::istream &operator>>(std::istream &is, model_lookup::model_map &mm);

void hepstats::likeconfig::process_stream() {
    std::string conf_line;

    while (std::getline(*conf_stream, conf_line)) {
        auto comment_pos = conf_line.find_first_of(comment_chars);

//      std::cerr << std::endl << "before='" << conf_line << "'" << std::endl;  // debugging
        conf_line = conf_line.substr(0, comment_pos);
//      std::cerr << "after='" << conf_line << "'" << std::endl;                // debugging

        std::istringstream parse(conf_line);

        model_lookup::model_map lookup_type;
        std::string lookup_code;
        likedist dist;
        double theory_err;
        bool theory_percent_err;

        if (!(parse >> lookup_type)
            || !(parse >> lookup_code)
            || !(parse >> dist)
            || !(parse >> theory_err)
            || !(parse >> std::boolalpha >> theory_percent_err)) {
//          std::cerr << "Rejecting this line!" << std::endl;                   // debugging
            continue;
        }

        double exp_val, exp_err;
        std::string data_filename;
        switch (dist) {
            case likedist::gaussian:
                if (!(parse >> exp_val) || !(parse >> exp_err))
                    continue;
                llhood.add_like_term(std::make_unique<gaussian>
                                             (model_lookup(lookup_type,
                                                           lookup_code),
                                              theory_err, theory_percent_err,
                                              exp_val, exp_err));
                break;

            case likedist::upper_gaussian:
                if (!(parse >> exp_val) || !(parse >> exp_err))
                    continue;
                llhood.add_like_term(std::make_unique<upper_gaussian>
                                             (model_lookup(lookup_type,
                                                           lookup_code),
                                              theory_err, theory_percent_err,
                                              exp_val, exp_err));
                break;

            case likedist::lower:
                if (!(parse >> exp_val) || !(parse >> exp_err))
                    continue;
                llhood.add_like_term(std::make_unique<smeared_lower_limit>
                                             (model_lookup(lookup_type,
                                                           lookup_code),
                                              theory_err, theory_percent_err,
                                              exp_val, exp_err));
                break;
        }
#if 0
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
#endif
    }
}

std::istream &operator>>(std::istream &is, hepstats::likedist &dist) {
    std::string dist_spec;
    is >> dist_spec;
    std::transform(dist_spec.begin(), dist_spec.end(), dist_spec.begin(),
                   ::tolower);

    if (dist_spec == "gaussian") {
        dist = hepstats::likedist::gaussian;
    } else if (dist_spec == "upper_gaussian") {
        dist = hepstats::likedist::upper_gaussian;
    } else if (dist_spec == "lower") {
        dist = hepstats::likedist::lower;
    } else if (dist_spec == "upper") {
        dist = hepstats::likedist::upper;
    } else if (dist_spec == "lower_interpolated") {
            dist = hepstats::likedist::lower_interpolated;
    } else if (dist_spec == "upper_interpolated") {
        dist = hepstats::likedist::upper_interpolated;
    } else {
        is.setstate(std::ios::failbit);
    }

    return is;
}

std::istream &operator>>(std::istream &is, model_lookup::model_map &mm) {
    std::string mm_spec;
    is >> mm_spec;
    std::transform(mm_spec.begin(), mm_spec.end(), mm_spec.begin(), ::tolower);

    if (mm_spec == "special")
        mm = model_lookup::special;
    else if (mm_spec == "output")
        mm = model_lookup::output;
    else if (mm_spec == "slha")
        mm = model_lookup::slha;
    else
        is.setstate(std::ios::failbit);

    return is;
}

