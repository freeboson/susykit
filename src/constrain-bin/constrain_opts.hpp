

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


#pragma once
#ifndef _CONSTRAINT_OPTS_H_
#define _CONSTRAINT_OPTS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <getopt.h>

#include "constrain/model_lookup.hpp"
#include "constraint.hpp"

#define SUJ_DEFAULT_VERSION 3

struct globalopts
{
	int version;
	std::string infile, outfile;
	std::vector<constraint> constraints;
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

int opthandle(int argc, char** argv, globalopts* gopts);

#endif

