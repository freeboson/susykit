

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


#include "libconstrain/hepstats.hpp"
#include <algorithm>
#include <sstream>

const std::string hepstats::likeconfig::comment_chars = "#";

void hepstats::likeconfig::process_stream()
{
	std::string conf_line;

	while (std::getline(*conf_stream, conf_line))
	{
		auto comment_pos = conf_line.find_first_of(comment_chars);

//		std::cerr << std::endl << "before='" << conf_line << "'" << std::endl; 	// debugging
		conf_line = conf_line.substr(0,comment_pos);
//		std::cerr << "after='" << conf_line << "'" << std::endl;		// debugging

		std::istringstream parse(conf_line);

		std::string lookup_type, lookup_code, dist_type;
		double exp_val, exp_err, theory_err;
		std::string theory_percent_err;
		if ( !(parse >> lookup_type)
			|| !(parse >> lookup_code)
			|| !(parse >> dist_type)
			|| !(parse >> exp_val)
			|| !(parse >> exp_err)
			|| !(parse >> theory_err)
			|| !(parse >> theory_percent_err) )
		{
//			std::cerr << "Rejecting this line!" << std::endl;		// debugging
			continue; 
		}
		std::transform(lookup_type.begin(),lookup_type.end(),lookup_type.begin(),::tolower);
		std::transform(dist_type.begin(),dist_type.end(),dist_type.begin(),::tolower);
		std::transform(theory_percent_err.begin(),theory_percent_err.end(),theory_percent_err.begin(),::tolower);

		model_lookup::model_map type;
		if (lookup_type == "special")
			type = model_lookup::special;
		else if (lookup_type == "output")
			type = model_lookup::output;
		else
			type = model_lookup::slha;

		likedist::distribution dist;
		if (dist_type == "lower")
			dist = likedist::Lower;
		else if (dist_type == "upper")
			dist = likedist::Upper;
		else if (dist_type == "poisson")
			dist = likedist::Poisson;
		else if (dist_type == "upper_gaussian")
			dist = likedist::Upper_Gaussian;
		else
			dist = likedist::Gaussian;


		llhood.add_like_term(
			likedatum(
				dist,
				model_lookup(type,lookup_code),
				exp_val, 
				exp_err, 
				theory_err,
				(bool)(theory_percent_err == "true")
			)
		);
	}
}


