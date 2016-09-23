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


#include "constrain/hepstats.hpp"
#include <memory>
#include <algorithm>
#include <sstream>

const std::string hepstats::likeconfig::comment_chars = "#";

void hepstats::likeconfig::process_stream() {
    std::string conf_line;

    while (std::getline(*conf_stream, conf_line)) {
        auto comment_pos = conf_line.find_first_of(comment_chars);

//		std::cerr << std::endl << "before='" << conf_line << "'" << std::endl; 	// debugging
        conf_line = conf_line.substr(0, comment_pos);
//		std::cerr << "after='" << conf_line << "'" << std::endl;		// debugging

        std::istringstream parse(conf_line);

        std::string lookup_type, lookup_code, dist_type;
        if (!(parse >> lookup_type)
            || !(parse >> lookup_code)
            || !(parse >> dist_type)) {
//			std::cerr << "Rejecting this line!" << std::endl;		// debugging
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

        switch(dist) {
            case likedist::lower:
            case likedist::upper:
            case likedist::poisson:
            case likedist::lower:
            case likedist::lower:
        }

        double exp_val, exp_err, theory_err;
        std::string theory_percent_err;

        std::transform(lookup_type.begin(), lookup_type.end(), lookup_type.begin(), ::tolower);
        std::transform(dist_type.begin(), dist_type.end(), dist_type.begin(), ::tolower);
        std::transform(theory_percent_err.begin(), theory_percent_err.end(), theory_percent_err.begin(), ::tolower);




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


