/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - constraint.cpp:                                                          -
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


#include "constraint.hpp"

using namespace std;

constraint::constraint()
        : lower(0), upper(0), lset(false), uset(false), xset(false) {
}

void constraint::process_limit(std::stringstream &ss) {
    string limit_token;
    for (int i = 0; ss >> limit_token; i++) {
        switch (i) {
            case 0:
                if (string::npos != limit_token.find("x")) {
                    i--; // optional exclusion parameter
                    xset = true;
                } else if (string::npos == limit_token.find("*")) {
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
                cerr << "Malformed limit on " << get_constraint_name() << ": "
                     << '"' << ss.str() << '"' << endl;
        }

    }
    if (xset && !(lset && uset))
        cerr << "Bad constraint: You must set upper and lower limits for "
                "exclusive limits!!"
             << endl;

}

// returns int which is bitset of which limits m fails
// where lower is the lsb, upper is the msb
int constraint::check_model(double value) const {
    int retflag = 0;
    if (!lset && !uset) {
        // no constraints? no problem! fail xset, pass !xset
        if (xset)
            retflag = 0x1 | 0x2;
        else
            retflag = 0x0;
    } else if (lset && uset) {
        retflag = 0x0;
        if (xset) { // exclude [lower,upper]
            if (value >= lower && value <= upper)
                retflag = 0x1 | 0x2;
        } else { // include [lower,upper]
            if (value < lower)
                retflag = retflag | 0x1;
            if (value > upper)
                retflag = retflag | 0x2;
        }
    } else if (lset) {
        if (xset && value >= lower)
            retflag = 0x1;
        else if (!xset && value < lower)
            retflag = 0x1;
    } else if (uset) {
        if (xset && value <= upper)
            retflag = 0x2;
        else if (!xset && value > upper)
            retflag = 0x2;
    }

//	cerr << "Real retflag=" << retflag << endl;
    return retflag;

}

int constraint::operator()(const model &m) const {
    return check_model(get_value(m));
}

string constraint::get_constraint() const {
    stringstream ss;

    ss << get_constraint_type();

    if (xset)
        ss << "excluding ";
    else
        ss << "requiring ";

    ss << get_constraint_name();

    if (lset && uset) {
        ss << " in " << "[" << lower << "," << upper << "]";
    } else if (lset) {
        ss << " >= " << lower;
    } else if (uset) {
        ss << " <= " << upper;
    } else
        ss << " of all values";

    return ss.str();
}

