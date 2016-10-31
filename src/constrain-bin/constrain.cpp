/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - constrain.cpp:                                                           -
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


#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "constraint.hpp"
#include "constrain_opts.hpp"
#include "constrain/model.hpp"
#include "constrain/parse.hpp"

#include "constrain/hepstats.hpp"

//#include "suj_dbg.h"

using namespace std;

class constrain {
public:
    constrain(istream *_i, ostream *_o, bool _merged, bool _cdb_file, istream *_cdb)
            : i(_i), o(_o), merged(_merged), cdb_file(_cdb_file), cdb(_cdb), mp() { }

    void operator()(vector<constraint>::iterator, vector<constraint>::iterator);

private:
    istream *i;
    ostream *o;
    bool merged;
    bool cdb_file;
    istream *cdb;
    model_parser mp;
};

int main(int argc, char **argv) {
    globalopts gopts;

    hepstats::loglike loglike;


    if (opthandle(argc, argv, &gopts)) {
        print_usage(argv[0]);
        return 1;
    }

    cerr << "original command:";
    for (int i = 0; i < argc; i++) {
        string arg(argv[i]);
        if (arg.find(' ') == string::npos)
            cerr << " " << arg;
        else
            cerr << " \"" << arg << "\"";
    }
    cerr << endl;


    cerr << "infile=" << gopts.infile << " " << "outfile=" << gopts.outfile << endl;
    if (gopts.print_cuts) {
        cerr << "Listing Constraints:" << endl;
        for (const auto &c : gopts.constraints)
            cerr << c.get_constraint() << endl;
    }

    if (gopts.pretend)
        return 0;

    if (gopts.constraints.size() < 1 && !gopts.force) {
        cerr << "No constraints requested... try cp?" << endl
        << "(Or, try using --force to update db format.)" << endl
        << endl;
        print_usage(argv[0]);
        return 1;
    }

    fstream f, fout;
    fstream cdb;

    if (!gopts.use_stdin) {
        f.open(gopts.infile.c_str(), fstream::in);
        if (!f.is_open() || f.fail()) {
            cerr << "Unable to open " << gopts.infile << " for reading!" << endl;
            return 1;
        }
    }

    if (!gopts.use_stdout) {
        if (gopts.append)
            fout.open(gopts.outfile.c_str(), fstream::out | fstream::app);
        else
            fout.open(gopts.outfile.c_str(), fstream::out | fstream::trunc);

        if (!fout.is_open() || fout.fail()) {
            if (f.is_open()) f.close();
            cerr << "Unable to open " << gopts.outfile << " for writing!" << endl;
            return 1;
        }
    }

    if (gopts.obs_file) {
        cdb.open(gopts.obs_filename.c_str(), fstream::in);
        if (!cdb.is_open() || cdb.fail()) {
            if (f.is_open()) f.close();
            if (fout.is_open()) fout.close();
            cerr << "Unable to open " << gopts.obs_filename << " for reading!" << endl;
            return 1;
        }
    }

    constrain con((gopts.use_stdin) ? &cin : &f,
                  (gopts.use_stdout) ? &cout : &fout,
                  gopts.merged_input,
                  gopts.obs_file,
                  &cdb
    );

    con(gopts.constraints.begin(), gopts.constraints.end());
    cerr << "Done!" << endl;
    cerr << endl;
    if (f.is_open()) f.close();
    if (fout.is_open()) fout.close();

    return 0;
}

void constrain::operator()(vector<constraint>::iterator first, vector<constraint>::iterator last) {
    string model_line;
    string cdb_line;
    for (int j = 0; getline(*i, model_line); j++) {
        bool isSafe = true;
        if (cdb_file) {
            if (!getline(*cdb, cdb_line)) {
                cerr << "Unexpected EOF in CDB!" << endl;
                return;
            }
            model_line += "\t";
            model_line += cdb_line;
            merged = true;
        }
        model m = mp.parse(model_line, merged);
        if (model::invalid == m.get_model_type()) {
            cerr << "bad model!" << endl;
            continue;
        }
        if (cdb_file && model::obs_invalid == m.get_observable_data_type()) {
//			cerr << "bad observables?" << endl;
            continue;
        }
        for (vector<constraint>::iterator it = first;
             it != last && isSafe;
             it++) {
            isSafe = (0 == it->operator()(m));
//			cerr << "retflag = " << it->operator()(m) << endl;
        }
        if (isSafe) {
            *o << m << endl;
        }
    }
}


