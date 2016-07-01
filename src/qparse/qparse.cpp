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

#include "constrain/model.hpp"
#include "../constrain/model_lookup.hpp"
#include "constrain/parse.hpp"
#include "../constrain/parseutils.hpp"

using namespace std;

// quick functor used for transform() by parse_models()
class perform_lookup {
public:
    perform_lookup(const model *_m) : m(_m) { }

    double operator()(const model_lookup &ml) const {
        return ml(*m);
    }

private:
    const model *m;
};

void usage(const string &s) {
    cerr << "Usage: " << s << " [models.db] [-M] [-i file.info] <-d | -s | -o > <datum code>" << endl;
}

int parse_reqs(int argc, const char **argv, int index, vector<model_lookup> *lookups, string *headers, bool &merged,
               string *infofile) {
    merged = true;
    stringstream ss;

    ss << "% ";

    if (index <= argc - 1) {
        string first = string(argv[index]);
        if (first == "-M") {
            merged = false;
            index++;
        }
    }

    while (index < argc - 1) // should have pairs only
    {
        string mode = string(argv[index++]);
        string code = string(argv[index++]);
        model_lookup ml;

        bool codeopt = true;

        if (mode.length() != 2 || mode[0] != '-') {
            cerr << "Invalid mode!" << endl;
            return 1;
        }

        switch (mode[1]) {
            case 'd':
                ml = model_lookup(model_lookup::slha, code);
                break;
            case 's':
                ml = model_lookup(model_lookup::special, code);
                break;
            case 'o':
                if (!merged) {
                    cerr << "You can't do -o with -M!" << endl;
                    return 1;
                }
                ml = model_lookup(model_lookup::output, code);
                break;
            case 'i':
                if (0 != infofile->length()) {
                    cerr << "Did you specify two info files? (Don't.)" << endl;
                    return 1;
                }
                *infofile = code;
                codeopt = false;
                break;
            default:
                cerr << "Bad code!" << endl;
                return 1;
        }

        if (codeopt) {
            ss << setw(DEFAULT_COL_WIDTH) << setiosflags(ios::left) << code << " ";

            if (ml.good_mode())
                lookups->push_back(ml);
            else
                return 1;
        }
    }

    *headers = ss.str();

    return 0;
}

int parse_models(const vector<model_lookup> &lookups, istream &reader, bool merged) {
    string model_line;
    model_parser mp;

    vector<double> values(lookups.size());

    while (getline(reader, model_line)) {
        model m = mp.parse(model_line, merged);
        transform(lookups.begin(), lookups.end(), values.begin(), perform_lookup(&m));
        cout << "  " << values << endl;
    }
    return 0;
}

int write_infofile(const string &headers, const string &infofile) {
    stringstream ss(headers), output;
    string word;

    fstream f;
    f.open(infofile.c_str(), fstream::out | fstream::trunc);
    if (!f.is_open() || f.fail()) return 1;

    ss >> word; // remove the %

    // set up the first few params
    output << "action = 5" << endl
    << "use_log= T" << endl
    << "use_data= 2" << endl;

    int params;
    for (params = 0; ss >> word; params++)
        output << "lab" << params + 1 << " =" << word << endl;

    f << "# Chain generated with SuperBayeS, v1.5, May 2010" << endl
    << endl
    << "params_saved= " << params << endl
    << output.str() << endl;

    f.close();
    return 0;
}

int main(int argc, const char **argv) {
    string fn;
    ifstream f;

    bool merged;

    vector<model_lookup> lookups;
    string headers;
    string infofile;

    int parse_fail;

    int index = 1; // ostensibly, next parameter
    if (argc < 3) {
        parse_fail = 1;
    }
    else {
        fn = string(argv[index]);
        bool read_stdin = (fn[0] == '-');

        if (read_stdin)
            cerr << "Reading models from stdin..." << endl;
        else
            index++;

        if (!(parse_fail = parse_reqs(argc, argv, index, &lookups, &headers, merged, &infofile))) {
            if (0 == infofile.length())
                cout << headers << endl;

            if (read_stdin) { // no models.db specified
                parse_fail = parse_models(lookups, cin, merged);
            }
            else {
                f.open(fn.c_str());
                if (f.fail()) {
                    cerr << "Error opening " << fn << " for reading!" << endl;
                    parse_fail = 1;
                }
                else {
                    parse_fail = parse_models(lookups, f, merged);
                }
            }
            if (infofile.length() > 0) {
                cerr << "Writing .info file..." << endl;
                write_infofile(headers, infofile);
            }
        }
    }

    if (parse_fail) {
        if (f.is_open()) f.close();
        usage(argv[0]);
        return 1;
    }

    if (f.is_open()) f.close();
    return 0;
}


