/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - model_lookup.cpp:                                                        -
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


#include <stdexcept>
#include "constrain/model_lookup.hpp"
#include "constrain/dict.hpp"
#include "constrain/special_lookups.hpp"
#include "constrain/parseutils.hpp"

// this will call the member function of the class of which "instance" is 
// an instance of, pointed to by "mem_fn_ptr"
// so, MEMBER_FN_PTR_CALL(model_a,get_input_ptr)("MSG") <=> (model_a.*get_input_ptr)("MSG")
#define MEMBER_FN_PTR_CALL(instance, mem_fn_ptr) ((instance).*(mem_fn_ptr))

using namespace std;
using namespace susy_dict;

model_lookup::model_lookup(model_map _mode, string _code)
        : mode(_mode), code(_code) {
    switch (mode) {
        case slha:
            action = &model::get_datum;
            break;

        case output:
            action = &model::get_observable;
            break;

        case special:
            action = nullptr;
            if (code == "NLSP") {
                cerr <<
                "NLSP codes: MC1 = 0; MSl1 = 1; Mst1 = 2; MHH or MH3 = 3; MNE2 = 4; MSG = 5; MSeR = 6; else 9" << endl;
            }
            break;

        default:
            throw domain_error("Unimplemented lookup mode!");
    }
}

double model_lookup::operator()(const model &m) const {
    if (mode != special) {
        return MEMBER_FN_PTR_CALL(m, action)(code);
    }
    else { // hack hack hack hack
        if (code == "LSP") {
            if (m.get_hierarchy(0) == m_o1)
                return 1;
            else
                return 0;

        }
        else if (code == "NLSP") {
            string nlsp;
            if (m.get_hierarchy(1) == m_o1) {
                nlsp = m.get_hierarchy(0);
            }
            else {
                nlsp = m.get_hierarchy(1);
            }
            if (nlsp == m_1pm)
                return 0;
            else if (nlsp == m_stau_1)
                return 1;
            else if (nlsp == m_t_1)
                return 2;
            else if (nlsp == m_H0 || nlsp == m_A0)
                return 3;
            else if (nlsp == m_o2)
                return 4;
            else if (nlsp == m_g)
                return 5;
            else if (nlsp == m_e_r)
                return 6;
            else
                return 9;
        }
        else if (code == "LSQ") {
            get_lsq lsq;
            return lsq(m);
        }
        else if (code == "LSL") {
            get_lsl lsl;
            return lsl(m);
        }
        else if (code == "RGG_approx") {
            get_RGG_approx rgg;
            return rgg(m);
        }
#if 0
        else if (code == "msp") {
            string mass;
            int next = 0;
//            int nd = 0; // what was this for?
            if (m.get_hierarchy(0) == m_o1) {
                next++;
            }
            else {
                cerr << m.get_hierarchy(0) << ";";
                mass = m.get_hierarchy(0);
                next = 2;
            }
            while (next < 5) {
                cerr << m.get_hierarchy(next++) << ";";
            }
            cerr << endl;
        }
#endif
        else {
            cerr << "Error: " << code << "is not a special action!" << endl;
            return 0;
        }

    }
}


