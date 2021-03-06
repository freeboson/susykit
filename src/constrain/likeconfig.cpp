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

std::istream &operator>>(std::istream &is, hepstats::likedist &dist);
std::istream &operator>>(std::istream &is, model_lookup::model_map &mm);

template<class ld>
static bool add_simple_term(std::istream *is, hepstats::loglike *ll,
                            model_lookup::model_map lt, std::string lc,
                            double pred_err, bool pct_err) {
    static_assert(std::is_base_of<hepstats::likedatum, ld>::value,
                  "specified ld is not derived from hepstats::likedatum.");
    double limit, limit_error;
    if (!(*is >> limit) || !(*is >> limit_error)) {
        return false;
    }
    ll->add_like_term(std::make_unique<ld>(
            model_lookup(lt, lc), pred_err, pct_err,
            std::make_unique<hepstats::simple_datum>(limit, limit_error)));
    return true;
}

template<class ld>
static bool add_interpolated_term(std::istream *is, hepstats::loglike *ll,
                                  model_lookup::model_map lt, std::string lc,
                                  double pred_err, bool pct_err) {
    static_assert(std::is_base_of<hepstats::likedatum, ld>::value,
                  "specified ld is not derived from hepstats::likedatum.");
    double limit_error;
    model_lookup::model_map lookup_axis_type;
    std::string lookup_axis_code, data_filename;
    if (!(*is >> lookup_axis_type) || !(*is >> lookup_axis_code)
        || !(*is >> data_filename) || !(*is >> limit_error)) {
        return false;
    }
    ll->add_like_term(std::make_unique<ld>(
            model_lookup(lt, lc), pred_err, pct_err,
            std::make_unique<hepstats::interpolated_data>(
                    model_lookup(lookup_axis_type, lookup_axis_code),
                    data_filename, limit_error)));
    return true;
}

auto
hepstats::likeconfig::operator()(std::istream *conf_stream) const -> loglike {

    loglike ll; // object to be returned, use move semantics plz
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
        double pred_error;
        bool pred_percent_err;

        if (!(parse >> lookup_type)
            || !(parse >> lookup_code)
            || !(parse >> dist)
            || !(parse >> pred_error)
            || !(parse >> std::boolalpha >> pred_percent_err)) {
//          std::cerr << "Rejecting this line!" << std::endl;                   // debugging
            continue;
        }

        // at the moment, continue = break, but leave it this way in case we
        // add stuff after the switch for some reason
        switch (dist) {
            case likedist::gaussian:
                if (!(add_simple_term<gaussian>(
                        conf_stream, &ll, lookup_type, lookup_code,
                        pred_error, pred_percent_err))) {
                    continue;
                }
                break;

            case likedist::upper_gaussian:
                if (!(add_simple_term<upper_gaussian>(
                        conf_stream, &ll, lookup_type, lookup_code,
                        pred_error, pred_percent_err))) {
                    continue;
                }
                break;

            case likedist::lower:
                if (!(add_simple_term<smeared_lower_limit>(
                        conf_stream, &ll, lookup_type, lookup_code,
                        pred_error, pred_percent_err))) {
                    continue;
                }
                break;

            case likedist::upper:
                if (!(add_simple_term<smeared_upper_limit>(
                        conf_stream, &ll, lookup_type, lookup_code,
                        pred_error, pred_percent_err))) {
                    continue;
                }
                break;

            case likedist::lower_interpolated:
                if (!(add_interpolated_term<smeared_lower_limit>(
                        conf_stream, &ll, lookup_type, lookup_code,
                        pred_error, pred_percent_err))) {
                    continue;
                }
                break;

            case likedist::upper_interpolated:
                if (!(add_interpolated_term<smeared_upper_limit>(
                        conf_stream, &ll, lookup_type, lookup_code,
                        pred_error, pred_percent_err))) {
                    continue;
                }
                break;
        }
    }
    return ll;
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

