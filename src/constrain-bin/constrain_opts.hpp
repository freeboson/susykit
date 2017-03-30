/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - constrain_opts.hpp:                                                      -
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
#ifndef CONSTRAINT_OPTS_HPP
#define CONSTRAINT_OPTS_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <getopt.h>

#include "constrain/model_lookup.hpp"
#include "constraint.hpp"

#define SUJ_DEFAULT_VERSION 3

struct globalopts {
    int version;
    std::string infile, outfile;
    std::vector< std::unique_ptr<constraint> > constraints;
    bool pretend, print_cuts, force, append;
    bool use_stdin, use_stdout;

    bool merged_input;
    bool obs_file;
    std::string obs_filename;

    bool likeconfig;
    std::string likeconfig_filename;
};

void print_usage(const std::string &s);

#ifdef SUJ_NO_STDOUT
std::string get_outfile(std::string infile);
#endif

int opthandle(int argc, char **argv, globalopts *gopts);

#endif

