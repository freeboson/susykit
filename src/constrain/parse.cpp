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


#include "constrain/parse.hpp"

#include <fstream>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

class make_slha_pair {
public:
    pair<string, double> operator()(const string &key, const double &datum) {
//		cout << "assigning " << datum << " to " << key << endl;
        return pair<string, double>(key, datum);
    }
};

class print_slha_data {
public:
    print_slha_data(ostream *_o, const model &_m) : o(_o), m(_m) { }

    void operator()(const string &key) {
        (*o) << "\t" << m.get_datum(key);
    }

private:
    ostream *o;
    model m;
};

class print_obs_data {
public:
    print_obs_data(ostream *_o, const model &_m) : o(_o), m(_m) { }

    void operator()(const string &key) {
        (*o) << "\t" << m.get_observable(key);
    }

private:
    ostream *o;
    model m;
};

unsigned long long count_lines(const string &fn) {
    string line;
    unsigned long long c;
    fstream f(fn.c_str(), fstream::in);
    if (f.fail()) return 0;
    for (c = 0; getline(f, line); c++);
    return c;
}

model stream_model_parser::operator()() {
    string line;
    getline(*i, line);
    model m = parse(line, merged);
    if (m.get_model_type() != model::invalid) count++;
    return m;
}

model model_parser::parse(const std::string &model_line, bool merged) const {

    istringstream iss;
    iss.str(model_line);

    string model_string;
    iss >> model_string;

//	cerr << "model_string = " << model_string << endl << endl;

    unsigned int expected_cols;
    list<string>::const_iterator row_begin, row_end;
    model::susy_model mtype;

    if (model_string == "mSUGRA") {
        mtype = model::mSUGRA;
        expected_cols = susy_dict::mSUGRA_row.size();
        row_begin = susy_dict::mSUGRA_row.begin();
        row_end = susy_dict::mSUGRA_row.end();
    }
    else {
        mtype = model::NUSUGRA;
        expected_cols = susy_dict::NUSUGRA_row.size();
        row_begin = susy_dict::NUSUGRA_row.begin();
        row_end = susy_dict::NUSUGRA_row.end();

    }

    streampos data_start = iss.tellg();

    unsigned int num_data = distance(istream_iterator<double>(iss),
                                     istream_iterator<double>());


    if (merged) {
        expected_cols += susy_dict::observable::observe_row.size();
    }

    if ((num_data + 1) != expected_cols || num_data < 1) // +1 to account for the "model"
    {
        cerr << "Error: malformed " << string((mtype == model::mSUGRA) ? "mSUGRA" : "NUSUGRA") <<
        string(merged ? "+OBSERVABLES" : "") << " line: "
        << num_data << " doubles provided, expecting " << (expected_cols - 1)
        << endl;
        return model();
    }

    iss.clear();
    iss.seekg(data_start);

    map<string, double> slha_data;

    transform(++row_begin,
              row_end,
              istream_iterator<double>(iss),
              inserter(slha_data, slha_data.begin()),
              make_slha_pair()
    );
    iss.unget();
    string cdb_line;
    if (merged) {
        getline(iss, cdb_line);
        return model(model_string, slha_data, mtype, cdb_line);
    }

    return model(model_string, slha_data, mtype);
}

std::ostream &operator<<(std::ostream &o, const model &m) {
    list<string>::const_iterator row_begin, row_end;

    switch (m.get_model_type()) {
        case model::mSUGRA:
            row_begin = susy_dict::mSUGRA_row.begin();
            row_end = susy_dict::mSUGRA_row.end();
            break;

        case model::NUSUGRA:
            row_begin = susy_dict::NUSUGRA_row.begin();
            row_end = susy_dict::NUSUGRA_row.end();
            break;

        case model::invalid:
        default:
            cerr << "Invalid model! Skipping..." << endl;
            return o;
            break;
    }

    ostringstream oss;
    oss << m.get_model_name();
    oss.setf(ios::scientific);
    oss.precision(8);
#if 1
    for_each(++row_begin,
             row_end,
             print_slha_data(&oss, m)
    );

//	if (model::obs_invalid != m.get_observable_data_type())
    if (m.get_observable(susy_dict::observable::micro_valid_bit)) {
        oss << "\t" << "1";
        for_each(++susy_dict::observable::observe_row.begin(),
                 susy_dict::observable::observe_row.end(),
                 print_obs_data(&oss, m)
        );
    }
#endif
    o << oss.str() << flush;

    return o;
}

