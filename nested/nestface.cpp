/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - nestface.cpp:                                                            -
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


#include "nestface.hpp"

// for create_dir stuff
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// errno stuff
#include <cerrno>
#include <cstring> // for strerror()
#include <sstream>

#include <iostream>
#include <fstream>
#include <sstream>

#include <forward_list>

#include <string>
//#include <regex>

hepstats::loglike loglike_calc;

//std::regex nl_match(R"(\n)");

void create_dir(const std::string &dir) {
    if (0 != mkdir(dir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
        if (EEXIST == errno) {
            std::cerr << dir << " exists... continuing..." << std::endl;
        }
        else {
            std::stringstream error_msg;
            error_msg << "Error creating " << dir << ": " << strerror(errno);
            throw (runtime_error(error_msg.str()));
        }
    }
    else {
        std::cerr << "Created directory " << dir << std::endl;
    }
}


void set_loglike_conf(const std::string &like_conf_name) {
    std::cerr << "Reading likelihood function configuration from " << like_conf_name << "..." << std::endl;

    std::ifstream reader;
    reader.open(like_conf_name.c_str());
    if (reader.fail()) {
        std::stringstream error_msg;
        error_msg << "Error opening " << like_conf_name << " for reading!";
        throw (runtime_error(error_msg.str()));
    }

    hepstats::likeconfig config(&reader);
    loglike_calc = config();

    if (reader.is_open())
        reader.close();
}

std::string get_full_root(const std::string &dir, const std::string &file_root) {
    std::stringstream full_root;

    full_root << dir << "/" << file_root;
    if (full_root.str().size() > 100) {
        throw length_error("Error: director+root larger than arbitrary cap. Feel free to change this.");
    }
    return full_root.str();
}

/******************************************** loglikelihood routine ****************************************************/

// Input arguments
// ndim 						= dimensionality (total number of free parameters) of the problem
// npars 						= total number of free plus derived parameters
// context						void pointer, any additional information
//
// Input/Output arguments
// Cube[npars] 						= on entry has the ndim parameters in unit-hypercube
//	 						on exit, the physical parameters plus copy any derived parameters you want to store with the free parameters
//	 
// Output arguments
// lnew 						= loglikelihood

//#define DEBUG

darksusy_driver darksusy;

void log_like_request(double *Cube, int &ndim, int &npars, double &lnew, void *context) {

    model m;
    lnew = -1e90;

    auto sugra = map_cube_to_opts(Cube);

#ifdef DEBUG

    std::forward_list<int> *pars_inputs = static_cast<std::forward_list<int>*>(context);

    std::stringstream point_summary;

    point_summary << "inputs = [ "
              << " --mtop='" << sugra->get_mtop_pole() << "'"
              << " --mbmb='" << sugra->get_mbmb() << "'"
              << " --alpha-s='" << sugra->get_alpha_s() << "'"
              << " --alpha-em-inv='" << sugra->get_alpha_em_inv() << "'"
              << " -- ";
    for (int index : *pars_inputs)
        point_summary << "'" << sugra->pars_at(index) << "' ";
    point_summary << "'" << sugra->get_tb() << "' " << "'" << sugra->get_sgnmu() << "' ]";

#endif

    softsusy_driver softsusy(sugra.get()); // softsusy_driver will just copy the contents
    feynhiggs_driver feynhiggs;
    superiso_driver superiso; // not ready yet

    try {

        m = softsusy(); // need to check for displayProblem().test() and neutralino LSP

    } catch (string s) {
#ifdef DEBUG
        s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
        std::cerr
            << point_summary.str() << " :: "
            << s << endl;
#endif
        return;
    }

    if (susy_dict::m_o1 != m.get_hierarchy(0)) {
        return;       // neutralino1 is not the LSP...
        // may use a tolerance in the
        // mass gap in the future
    }

    double softsusy_higgs = m.get_datum(susy_dict::m_h0);

    try {

        feynhiggs(&m);
        darksusy(&m);
        superiso(&m);

    } catch (exception &e) {

        std::cerr << "Observables error :: " << e.what() << std::endl;
        return;

    }


    lnew = loglike_calc.get_log_like(m);

#ifdef DEBUG

    get_lsl lsl;

    point_summary << " :: ln(like) = " << lnew;

    point_summary << " :: outputs = ["
              << " m_h0: " << m.get_datum(susy_dict::m_h0)
              << " omega: " << m.get_observable(susy_dict::observable::omega)
              << " m_o1: " << m.get_datum(susy_dict::m_o1)
              << " m_1pm: " << m.get_datum(susy_dict::m_1pm)
              << " m_o2: " << m.get_datum(susy_dict::m_o2)
              << " m_lsl: " << lsl(m)
              << " ]";

    std::cerr << point_summary.str() << std::endl;

#else

    std::forward_list<int> *pars_inputs = static_cast<std::forward_list<int> *>(context);

    std::stringstream point_summary;
    point_summary << "ln(like) = " << lnew << " :: ";

    point_summary << "inputs = [ "
    << " --mtop='" << sugra->get_mtop_pole() << "'"
    << " --mbmb='" << sugra->get_mbmb() << "'"
    << " --alpha-s='" << sugra->get_alpha_s() << "'"
    << " --alpha-em-inv='" << sugra->get_alpha_em_inv() << "'"
    << " -- ";
    for (int index : *pars_inputs)
        point_summary << "'" << sugra->pars_at(index) << "' ";
    point_summary << "'" << sugra->get_tb() << "' " << "'" << sugra->get_sgnmu() << "' ]";

    get_lsl lsl;

    point_summary << " :: outputs = ["
    << " m_h0: " << m.get_datum(susy_dict::m_h0)
    << " omega: " << m.get_observable(susy_dict::observable::omega)
    << " proton_SI: " << (m.get_observable(susy_dict::observable::proton_SI) * 1e-36)
    << " m_o1: " << m.get_datum(susy_dict::m_o1)
    << " m_1pm: " << m.get_datum(susy_dict::m_1pm)
    << " m_o2: " << m.get_datum(susy_dict::m_o2)
    << " m_lsl: " << lsl(m)
    << " ]";

    std::cerr << point_summary.str() << std::endl;

#endif

    auto *slha_row = &susy_dict::mSUGRA_row;
    if (sugra->is_nusugra())
        slha_row = &susy_dict::NUSUGRA_row;

    auto row_begin = slha_row->begin();

    std::transform(
            ++row_begin, // skip the model name
            slha_row->end(),
            Cube + ndim,             // these start after free params
            [&m](const std::string &key) -> double {
                return m.get_datum(key);
            }
    );

    std::transform(
            susy_dict::observable::observe_row.begin(),
            susy_dict::observable::observe_row.end(),
            Cube + ndim + (slha_row->size() - 1),
            [&m](const std::string &key) -> double {
                return m.get_observable(key);
            }
    );

}


