/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - model.hpp:                                                               -
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

#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>

#include "dict.hpp"

#if 0
class observable_data
{
public:
    enum data_type
    { // using an enum to support different versions in the future
        valid,
        invalid
    };

    observable_data() :type(invalid) {}
    observable_data(const std::string &obs_dat_line);

    data_type get_data_type() const { return type; }

    double get_datum(const std::string &datum_key) const;

private:
    data_type type;
    std::map<std::string,double> obs_data_set;
};
#endif

class model {
public:
    enum susy_model {
        invalid,
        mSUGRA,
        NUSUGRA
    };
    enum data_type { // using an enum to support different versions in the future
        valid,
        obs_invalid
    };

    model() : model_name(""), model_type(invalid), obs_type(obs_invalid) { }

    model(std::string _model_name, std::map<std::string, double> _slha_data, susy_model _model_type);

    model(std::string _model_name, std::map<std::string, double> _slha_data, susy_model _model_type,
          const std::string &obs_dat_line);

    std::string get_model_name() const { return model_name; }

    bool hierarchy_cmp(const model &m, const int &n) const;

    std::string get_hierarchy(size_t n) const;

    const std::multimap<double, std::string> &get_hierarchy() const { return hierarchy; }

    std::multimap<double, std::string>::const_iterator get_hierarchy_first() const { return hierarchy.begin(); }

    std::multimap<double, std::string>::const_iterator get_hierarchy_last() const { return hierarchy.end(); }

    const std::map<std::string, double> &get_spectrum() const { return spectrum; }

    std::map<std::string, double>::const_iterator get_spectrum_first() const { return spectrum.begin(); }

    std::map<std::string, double>::const_iterator get_spectrum_last() const { return spectrum.end(); }

    double get_datum(const std::string &datum_key) const;

    double get_observable(const std::string &obs_datum_key) const;

    void set_datum(const std::string &key, double value) {
        slha_data[key] = value;
        build_spectrum();
    } // happy to overwrite

    void set_observables(const std::string &cdb_line);

    void set_observable(const std::string &key, double value) { observables[key] = value; } // happy to overwrite

    std::pair<std::string, double> get_mass_pair(const std::string &pname) const;

    std::string get_slha() const;

    void write_slha(std::ostream &out) const;

    susy_model get_model_type() const { return model_type; }

    data_type get_observable_data_type() const { return obs_type; }

private:
    std::string model_name;
    std::map<std::string, double> slha_data;
    std::map<std::string, double> observables;
    std::map<std::string, double> spectrum;
    std::multimap<double, std::string> hierarchy;

    void build_spectrum();

    void build_hierarchy();

    void fix_minpar_keys();

    void blank_observables();

    susy_model model_type;
    data_type obs_type;
};

std::ostream &operator<<(std::ostream &o, const std::pair<std::string, double> &p);

std::ostream &operator<<(std::ostream &o, const std::pair<double, std::string> &p);

#endif


