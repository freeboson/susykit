

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


#include <iostream>
#include <fstream>
#include <string>

#include "constrain/dict.hpp"
#include "constrain/model.hpp"
#include "constrain/parse.hpp"
#include "constrain/get_slha.hpp"
#include "constrain/special_lookups.hpp"

#include "constrain/hepstats.hpp"

using namespace std;

int main(int argc, char** argv)
{
	model_parser mp;
	hepstats::loglike llhood;
	ifstream reader;

	if (2 == argc)
	{
		string filename = argv[1];
		cerr << "Reading likelihood function configuration from " << filename << "..." << endl;
		reader.open(filename.c_str());
		if (reader.fail())
		{
			cerr << "Error opening " << filename << " for reading!" << endl;
			return 1;
		}
		hepstats::likeconfig config(&reader);
		llhood = config();
		if (reader.is_open()) 
			reader.close();

		if (llhood.get_num_like_terms() < 1)
		{
			cerr << "Not able to read any good conf lines from " << reader << "!" << endl;
			return 2;
		}
	}
	else
	{
		cerr << "Using hard-coded likelihood function defined in test.cpp..." << endl;
		llhood.add_like_term(
			hepstats::likedatum(
				hepstats::likedist::Gaussian,
				model_lookup(model_lookup::slha, susy_dict::m_h0),
				125.0, //exp value
				0.5,   //exp error
				0.5,   //th. error
				false) //th. error %-error?
		);
	}

	string model_line;
	while (getline(cin,model_line))
	{
		model m = mp.parse(model_line);
		cout << "ln(like) = " << llhood.get_log_like(m) << endl;
	}

	if (reader.is_open()) reader.close();

	return 0;
}


