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



#include "constraint.hpp"
#include <stdexcept>

using namespace std;

constraint::constraint()
        : lower(0), upper(0), lset(false), uset(false), xset(false) {

}

constraint::constraint(const model_lookup &_ml, const string &cons_line)
        : ml(_ml), lower(0), upper(0), lset(false), uset(false), xset(false), like_constraint(false) {
    stringstream cline(cons_line);
    cline >> param;
    process_limit(cline);
}

constraint::constraint(const hepstats::loglike &_loglike, const std::string &limit)
        : loglike(_loglike), lower(0), upper(0), lset(false), uset(false), xset(false), like_constraint(true) {
    param = "[chi^2 = -2ln(like)]";
    stringstream limit_ss(limit);
    process_limit(limit_ss);
}


void constraint::process_limit(std::stringstream &ss) {
    string limit_token;
    for (int i = 0; ss >> limit_token; i++) {
        switch (i) {
            case 0:
                if (string::npos != limit_token.find("x")) {
                    i--; // optional exclusion parameter
                    xset = true;
                }
                else if (string::npos == limit_token.find("*")) {
                    lset = true;
                    try {
                        lower = stod(limit_token);
                    } catch (const std::logic_error &) {
                        lset = false;
                    }
                }
                break;
            case 1:
                if (string::npos == limit_token.find("*")) {
                    uset = true;
                    try {
                        upper = stod(limit_token);
                    } catch (const std::logic_error &) {
                        uset = false;
                    }
                }
                break;
            default:
                cerr << "Malformed limit on " << param << ": \"" << ss.str() << "\"" << endl;
                param = "BAD_CONSTRAINT!";
        }

    }
    if (xset && !(lset && uset))
        cerr << "Bad constraint: You must set upper and lower limits for exclusive limits!!" << endl;

}

// returns int which is bitset of which limits m fails
// where lower is the lsb, upper is the msb
int constraint::check_model(double value) const {
    if (!ml.good_mode() && !like_constraint) {
        cerr << "model_lookup not set!" << endl;
        return 0x4;
    }

    int retflag = 0;
    if (!lset && !uset) {
        // no constraints? no problem! fail xset, pass !xset
        if (xset)
            retflag = 0x1 | 0x2;
        else
            retflag = 0x0;
    }
    else if (lset && uset) {
        retflag = 0x0;
        if (xset) { // exclude [lower,upper]
            if (value >= lower && value <= upper)
                retflag = 0x1 | 0x2;
        }
        else { // include [lower,upper]
            if (value < lower)
                retflag = retflag | 0x1;
            if (value > upper)
                retflag = retflag | 0x2;
        }
    }
    else if (lset) {
        if (xset && value >= lower)
            retflag = 0x1;
        else if (!xset && value < lower)
            retflag = 0x1;
    }
    else if (uset) {
        if (xset && value <= upper)
            retflag = 0x2;
        else if (!xset && value > upper)
            retflag = 0x2;
    }

//	cerr << "Real retflag=" << retflag << endl;
    return retflag;

}

int constraint::operator()(const model &m) const {
    if (like_constraint) {
//		cerr << "What am I doing here?" << endl;
        return check_model(-2 * loglike.get_log_like(m));
    }
    else {
//		cerr << "Evaluating constraint..." << endl;
        return check_model(ml(m));
    }
}

double constraint::get_value(const model &m) const {
    if (like_constraint)
        return loglike.get_log_like(m);

    if (!ml.good_mode()) {
        cerr << "model_lookup not set!" << endl;
        return 0;
    }
    return ml(m);
}

string constraint::get_constraint() const {
    stringstream ss;

    if (like_constraint) {
        ss << "Advanced statistical ";
    }
    else {
        switch (ml.get_mode()) {
            case model_lookup::slha:
                ss << "Basic ";
                break;
            case model_lookup::output:
                ss << "Output ";
                break;
            case model_lookup::special:
                ss << "Special ";
                break;
            default:
                ss << "Unspecified (bad) ";
        }
    }

    ss << "constraint ";

    if (xset)
        ss << "excluding ";
    else
        ss << "requiring ";

    ss << param;

    if (lset && uset) {
        ss << " in " << "[" << lower << "," << upper << "]";
    }
    else if (lset) {
        ss << " >= " << lower;
    }
    else if (uset) {
        ss << " <= " << upper;
    }
    else
        ss << " of all values";

    return ss.str();
}


