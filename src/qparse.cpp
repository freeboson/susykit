

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
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <functional>
#include <cmath>
#include <list>

#include "model.hpp"
#include "model_lookup.hpp"
#include "parse.hpp"
#include "parseutils.hpp"

using namespace std;

// quick functor used for transform() by parse_models()
class perform_lookup
{
public:
	perform_lookup(const model *_m) :m(_m) {}
	double operator()(const model_lookup &ml) const
	{
		return ml(*m);
	}
private:
	const model *m;
};

void usage(const string &s)
{
	cerr << "Usage: " << s << " [models.db] [-m] <-d | -s | -o > <datum code>" << endl;
}

int parse_reqs(int argc, const char** argv, int index, vector<model_lookup> *lookups, string *headers, bool &merged)
{
	merged = false;
	stringstream ss;

	ss << "% ";

	if (index <= argc-1)
	{
		string first = string(argv[index]);
		if (first == "-m")
		{
			merged = true;
			index++;
		}
	}

	while(index < argc - 1) // should have pairs only
	{
		string mode = string(argv[index++]);
		string code = string(argv[index++]);
		model_lookup ml;

		if (mode.length() != 2 || mode[0] != '-')
		{
			cerr << "Invalid mode!" << endl;
			return 1;
		}

		switch(mode[1])
		{
			case 'd':
				ml = model_lookup(model_lookup::slha, code);
				break;
			case 's':
				ml = model_lookup(model_lookup::special, code);
				break;
			case 'o':
				if (!merged)
				{
					cerr << "You can't do -o without -m!" << endl;
					return 1;
				}
				ml = model_lookup(model_lookup::output, code);
				break;
			default:
				cerr << "Bad code!" << endl;
				return 1;
		}

		ss << setw(DEFAULT_COL_WIDTH) << setiosflags(ios::left) << code << " ";

		if (ml.good_mode())
		{
			lookups->push_back(ml);
		}
		else
		{
			return 1;
		}
	}

	*headers = ss.str();

	return 0;
}

int parse_models(const vector<model_lookup> &lookups, istream &reader, bool merged)
{
	string model_line;
	model_parser mp;

	vector<double> values(lookups.size());

	while (getline(reader,model_line))
	{
		model m = mp.parse(model_line,merged);
		transform(lookups.begin(),lookups.end(),values.begin(),perform_lookup(&m));
		cout << "  " << values << endl;
	}
	return 0;
}

int main(int argc, const char**argv)
{
	string fn;
	ifstream f;

	bool merged;

	vector<model_lookup> lookups;
	string headers;

	int parse_fail;

	int index = 1; // ostensibly, next parameter
	if (argc < 3)
	{
		parse_fail = 1;
	}
	else
	{
		fn = string(argv[index]);
		bool read_stdin = (fn[0] == '-');

		if (read_stdin)
			cerr << "Reading models from stdin..." << endl;
		else
			index++;

		if (!(parse_fail = parse_reqs(argc, argv, index, &lookups, &headers, merged)))
		{
			cout << headers << endl;
			if (read_stdin)
			{ // no models.db specified
				parse_fail = parse_models(lookups, cin, merged);
			}
			else
			{
				f.open(fn.c_str());
				if (f.fail())
				{
					cerr << "Error opening " << fn << " for reading!" << endl;
					parse_fail = 1;
				}
				else
				{
					parse_fail = parse_models(lookups, f, merged);
				}
			}
		}
	}

	if (parse_fail)
	{
		if (f.is_open()) f.close();
		usage(argv[0]);
		return 1;
	}

	if (f.is_open()) f.close();
	return 0;
}


