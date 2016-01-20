

/*

   **************************************************************
   *                                                            *
   * Seed Generator                                             *
   * Sujeet Akula                                               *
   * sujeet@freeboson.org                                       *
   *                                                            *
   * Simple program to print a list of n 6-digit seeds,         *
   * where n is specified at the command line. Boost is         *
   * necessary for compilation.                                 *
   *                                                            *
   * To compile:                                                *
   *                                                            *
   *    g++ -I/path/to/boost/ gen_seeds.cpp -o gen_seeds        *
   *                                                            *
   * To run:                                                    *
   *                                                            *
   *    ./gen_seeds 30 > seeds.txt                              *
   *    (seeds.txt will be a file with 30 seeds)                *
   *                                                            *
   **************************************************************

*/

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>

#define NUM_REQ_ARGS 2
#define NUM_MAX_SEEDS 100000

using namespace std;

class gen_seed
{
public:
	gen_seed(int seed)
	:dist(100000,999999)
	{
		gen.seed(seed);
	}

	unsigned int operator()()
	{
		return dist(gen);
	}
private:
	mt19937 gen;
	uniform_int_distribution<int> dist;
};

class seed_printer
{
public:
	seed_printer(ostream *o) :out(o) {}
	void operator()(unsigned int x)
	{
		*out << x << endl;
	}
private:
	ostream *out;
};

void usage(const string &s)
{
	cerr << "Usage: " << s << " <# of 6-digit seeds to generate>" << endl;
}

int main(int argc, char** argv)
{
	if (NUM_REQ_ARGS != argc)
	{
		usage(argv[0]);
		return 1;
	}

	unsigned int nseeds;

	stringstream ss(argv[1]);
	ss >> nseeds;

	if (nseeds > NUM_MAX_SEEDS)
	{
		cerr 	<< "Error: it's not a good idea to try to generate " 
			<< nseeds << " random 6-digit seeds." << endl
			<< "You should modify the code to suit your needs." << endl
			<< endl;
		usage(argv[1]);
		return 2;
	}

	cerr << "Generating " << nseeds << " unique seeds..." << endl;

	vector <unsigned int> seeds(nseeds);
	
	vector<unsigned int>::iterator unique_end = seeds.begin();
	do {
		generate(unique_end, seeds.end(),gen_seed(time(NULL)));
		sort(seeds.begin(),seeds.end());
		unique_end = unique(seeds.begin(),seeds.end());
	} while (unique_end != seeds.end());

	for_each(seeds.begin(),seeds.end(),seed_printer(&cout));

	return 0;
}

