/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - gen_seeds.cpp: This is a defunct component.                              -
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

#include <boost/version.hpp>
#include <boost/random/mersenne_twister.hpp>

#if (BOOST_VERSION >= 104700)

#include <boost/random/uniform_int_distribution.hpp>

typedef boost::random::uniform_int_distribution<> uniform_int_dist;
typedef boost::random::mt19937 boost_mt19937;
#else
#include <boost/random/uniform_int.hpp>
typedef boost::uniform_int<> uniform_int_dist;
typedef boost::mt19937 boost_mt19937;
#endif

#define NUM_REQ_ARGS 2
#define NUM_MAX_SEEDS 100000

using namespace std;

class gen_seed {
public:
    gen_seed(int seed)
            : dist(100000, 999999) {
        gen.seed(seed);
    }

    unsigned int operator()() {
        return dist(gen);
    }

private:
    boost_mt19937 gen;
    uniform_int_dist dist;
};

class seed_printer {
public:
    seed_printer(ostream *o) : out(o) { }

    void operator()(unsigned int x) {
        *out << x << endl;
    }

private:
    ostream *out;
};

void usage(const string &s) {
    cerr << "Usage: " << s << " <# of 6-digit seeds to generate>" << endl;
}

int main(int argc, char **argv) {
    if (NUM_REQ_ARGS != argc) {
        usage(argv[0]);
        return 1;
    }

    unsigned int nseeds;

    stringstream ss(argv[1]);
    ss >> nseeds;

    if (nseeds > NUM_MAX_SEEDS) {
        cerr << "Error: it's not a good idea to try to generate "
        << nseeds << " random 6-digit seeds." << endl
        << "You should modify the code to suit your needs." << endl
        << endl;
        usage(argv[1]);
        return 2;
    }

    cerr << "Generating " << nseeds << " unique seeds..." << endl;

    vector<unsigned int> seeds(nseeds);

    vector<unsigned int>::iterator unique_end = seeds.begin();
    do {
        generate(unique_end, seeds.end(), gen_seed(time(NULL)));
        sort(seeds.begin(), seeds.end());
        unique_end = unique(seeds.begin(), seeds.end());
    } while (unique_end != seeds.end());

    for_each(seeds.begin(), seeds.end(), seed_printer(&cout));

    return 0;
}

