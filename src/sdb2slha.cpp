

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
#include <sstream>

#include "model.hpp"
#include "parse.hpp"
#include "parseutils.hpp"
#include "get_slha.hpp"

using namespace std;

void usage(const string &s)
{
	cerr << "Usage: " << s << " <models.mdb> <line number>" << endl;
}

model get_model(istream &reader, unsigned int line_num)
{
	string model_line;
	#if 0
	model_parser mp;

	for (unsigned int i = 1; getline(reader,model_line); i++)
	{
		if (i == line_num)
		{
			return mp.parse(model_line,true);
		}
	}
	return model();
	#else
	  #if 0
	stream_model_parser get_model(&reader,true);
	model m; 
	for (unsigned int i = 1; i < line_num; i++) get_model();
	return get_model();
	  #else
	    
	for (unsigned int i = 1; i < line_num; i++) getline(reader,model_line);
	stream_model_parser get_model(&reader,true);
	return get_model();
	  #endif
	#endif

}

int main(int argc, const char**argv)
{
	string fn;
	ifstream f;
	unsigned int line_num;
	get_slha slha;

	if (argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	else
	{
		fn = string(argv[1]);
		stringstream ss(argv[2]);
		ss >> line_num;

		f.open(fn.c_str());
		if (f.fail())
		{
			usage(argv[0]);
			cerr << "Error opening " << fn << " for reading!" << endl;
			return 2;
		}
		model m = get_model(f, line_num);
		if (model::invalid == m.get_model_type())
		{
			usage(argv[0]);
			cerr << "Invalid model!" << endl;
			return 3;

		}
		if (f.is_open()) f.close();

		cout << slha(m) << flush;
	}

	if (f.is_open()) f.close();
	return 0;
}


