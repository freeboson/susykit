/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - constrain_opts.cpp:                                                      -
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


#include "constrain_opts.hpp"
#include <cstring>
#include <algorithm>
#include <fstream>

//extern char *optarg;
//extern int optind, opterr, optopt;

using namespace std;

string get_cline_code(const string &cline) {
    string code;
    stringstream ss(cline);
    ss >> code;
    return code;
}

void print_usage(const string &s) {
    cerr << "Constrain-1.1 by Sujeet Akula" << endl
    << "sujeet@freeboson.org" << endl
    << endl
    << "Usage: " << s <<
    " [-alpf] [ -L <loglike.conf> ] [ -M | [ -m | --obs <observables.cdb> ] ] [EXPR1 [EXPR2 [...]]] [inputfile.sdb [outputfile.mdb]]" <<
    endl
    << "Where, EXPR's are either predefined constraints or determined by a custom constraint line" << endl
    << "\t" << "EXPR is < --cut | --output | --special> <CLINE> OR < --chi2 LIMIT > " << endl
    << "\t" <<
    "CLINE is \"<param> LIMIT\" and LIMIT is [x] <lower | #> <upper | #> (you will want to pass CLINE as a single string)" <<
    endl
    << "\t" <<
    "where [lower,upper] is the relevant range imposed on param, and x switches from inclusive (default) to exclusive." <<
    endl
    << "\t" << "The codes for the CLINES are in dict.h; feel free to adjust them to your comfort." << endl
    << "\t" << "Special CLINE's are hacks, but the codes include: NLSP, LSQ, and LSL. (Look in perform_lookup.cpp.)" <<
    endl

    << endl
    << "Example: " << s << " --obs observables.cdb --cut \"m_g 100 800\" --output \"Omega # 0.14\" in.sdb" << endl
    << "Here, models will be read from in.sdb, and the observables will be read in from observables.cdb. Since only " <<
    endl
    <<
    "one filename is given at the end, models that have a gluino mass betwen 100 and 800 GeV and relic density less " <<
    endl
    << "than or equal to 0.14 will be printed to stdout (feel free to redirect to a file)." << endl
    << endl
    << "Example: " << "bzcat in.mdb.bz2 | " << s <<
    " -m --cut \"m_g 100 800\" --output \"Omega # 0.14\" | bzip2 --best > out.mdb.bz2" << endl
    << "This imposes the same constraints on your database as before, but here, you have your SLHA data" << endl
    << "and your calculated observables merged in the same file (presumably by previously using the --obs" << endl
    << "flag). Additionally, instead of specifying input and output filenames, the use of pipes deliver" << endl
    << "data to your program through stdin, and the constrained data through stdout. Finally, in this " << endl
    << "example, the input and output data sets use bzip(2) compression." << endl
    << endl
    << "If unsure, add -p to your command-line as a dry run." << endl
    << endl
    << "Options: " << endl
    << "\t" << "Observables:" << endl
    << "\t\t" <<
    "-m, this implies that your data has been `muxed' with the observable calculations (default, obsolete)" << endl
    << "\t\t" << "-M, this implies that your data has NOT been `muxed' with the observable calculations" << endl
    << "\t\t" << "--obj=file.cdb, a list of observable calculations that parallels your data set must" << endl
    << "\t\t" << "be given in file.cdb. The output will `mux' the SLHA and observable data for you." << endl
    << endl
    << "\t\t" <<
    "-L, --likeconfig=likelihood.conf, this specifies the configuration file for the likelihood function (some lookups may need --obs or -m)" <<
    endl
    << endl
    << "\t" << "Constraints:" << endl
    << "\t\t" << "-x, --cut=CLINE, apply limits defined in CLINE to an SLHA datum" << endl
    << "\t\t" <<
    "-X, --chi2=LIMIT, apply limits defined in LIMIT (as in CLINE, without <param>) to chi^2 = -2ln(like) (needs -L)" <<
    endl
    << "\t\t" << "-o, --output=CLINE, apply limits defined in CLINE to an observable (requires -m or --obs)" << endl
    << "\t\t" << "-s, --special=CLINE, apply limits defined in CLINE to a specially calculated quantity" << endl
    << "\t\t" << "Special lookups include LSQ, LSL, and NLSP." << endl
    << endl
    << "\t\t" << "-b, --bphys, preset constraints including SUSY corrections to b-physics and (g-2)_\\mu" << endl
    //		<< "\t\t" << "-B, --bphys-misiak, same as -b, except the limits on Br(B^0_s -> mu+ mu-) are shifted by the NNLO SM Misiak correction" << endl // I want to phase this out
    << "\t\t" << "-P, --pdg, preset constraints on sparticle masses (not implemented)" << endl
    << endl
    << "\t" << "Miscellaneous:" << endl
    << "\t\t" << "-h, --help, prints this list" << endl
    << "\t\t" << "-a, --append, append to output file instead of truncating" << endl
    << "\t\t" << "-l, --list-constraints, print cuts before constraining models" << endl
    << "\t\t" << "-p, --pretend, only display cuts and parameters... (do not do any other I/O)" << endl
    << "\t\t" << "-f, --force, process data even if no constraints are given" << endl
    << endl
    << "If no files are specified, stdin and stdout will be used." << endl
    << endl;
}

int opthandle(int argc, char **argv, globalopts *gopts) {
    gopts->version = SUJ_DEFAULT_VERSION;
    static option longopts[] = {
            // help
            {"help",             no_argument,       NULL, 'h'},
            {"list-constraints", no_argument,       NULL, 'l'},
            {"pretend",          no_argument,       NULL, 'p'},
            // special
            {"force",            no_argument,       NULL, 'f'},
            {"append",           no_argument,       NULL, 'a'},
            {"merged",           no_argument,       NULL, 'm'},
            {"obs",              required_argument, NULL, 'O'},
            {"likeconfig",       required_argument, NULL, 'L'},
            // cmd line constraints
            {"cut",              required_argument, NULL, 'x'},
            {"chi2",             required_argument, NULL, 'X'},
            {"output",           required_argument, NULL, 'o'},
            {"special",          required_argument, NULL, 's'},
            // preset constraints
            {"bphys",            no_argument,       NULL, 'b'},
            {"bphys-misiak",     no_argument,       NULL, 'B'},
            {"pdg",              no_argument,       NULL, 'P'},
            {NULL,               no_argument,       NULL, 0}
    };

    static const char *optcstr = "hlpfamMO:L:x:X:o:s:bBP";

    bool need_observables = false;
    bool need_likeconfig = false;
    vector<string> like_limits;

    int longindex = 0;

    // default options
    gopts->print_cuts = false;
    gopts->pretend = false;
    gopts->force = false;
    gopts->append = false;
    gopts->merged_input = true; // -m is now default!!
    gopts->obs_file = false;
    gopts->obs_filename = string("");
    gopts->likeconfig = false;
    gopts->likeconfig_filename = string("");

    int opt;
    stringstream ss;
    double bsgnlo_accepted = 3.52e-4,        // Accepted b->s\gamma, NLO
            bsgnlo_delta = (3.60e-4 - 3.15e-4),    // Misiak NNLO Correction
            bsgnlo_lim = 3 * 0.25e-4;            // 3\sigma

    // preset CLINES for PDG limits
    const string higgs = susy_dict::m_h0 + " 114 #";
    const string neutralino1 = susy_dict::m_o1 + " 46 #";
    const string chargino1 = susy_dict::m_1pm + " 94 #";
    const string selectronr = susy_dict::m_e_r + " 107 #";
    const string smuonr = susy_dict::m_mu_r + " 94 #";
    const string stau1 = susy_dict::m_stau_1 + " 81.9 #";
    const string sbottom1 = susy_dict::m_b_1 + " 89 #";
    const string stop1 = susy_dict::m_t_1 + " 95.7 #";
    const string gluino = susy_dict::m_g + " 308 #";
    const string squark = "LSQ 379 #";

    while (-1 != (opt = getopt_long(argc, argv, optcstr, longopts, &longindex))) {
        switch (opt) {
            case 'x':
                if (NULL == optarg) {
                    cerr << "getopt_long() error!!!" << endl;
                    return 1;
                }
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(string(optarg))), string(optarg))
                );
                break;

            case 's':
                if (NULL == optarg) {
                    cerr << "getopt_long() error!!!" << endl;
                    return 1;
                }
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::special, get_cline_code(string(optarg))), string(optarg))
                );
                break;

            case 'o':
                if (NULL == optarg) {
                    cerr << "getopt_long() error!!!" << endl;
                    return 1;
                }
                need_observables = true;
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, get_cline_code(string(optarg))), string(optarg))
                );
                break;

            case 'm':
                // does not do anything anymore
                break;

            case 'M':
                gopts->merged_input = false;
                break;

            case 'O':
                if (NULL == optarg) {
                    cerr << "getopt_long() error!!!" << endl;
                    return 1;
                }
                gopts->merged_input = false;
                gopts->obs_file = true;
                gopts->obs_filename = string(optarg);
                break;

            case 'L':
                if (NULL == optarg) {
                    cerr << "getopt_long() error!!!" << endl;
                    return 1;
                }
                gopts->likeconfig = true;
                gopts->likeconfig_filename = string(optarg);
                break;

            case 'X':
                if (NULL == optarg) {
                    cerr << "getopt_long() error!!!" << endl;
                    return 1;
                }
                like_limits.push_back(string(optarg));
                need_likeconfig = true;
                break;

            case 'B':

                need_observables = true;

                ss.str("");
                ss.clear();
                ss << susy_dict::observable::bsgnlo << (bsgnlo_accepted + bsgnlo_delta - bsgnlo_lim) << " " <<
                (bsgnlo_accepted + bsgnlo_delta + bsgnlo_lim);
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, get_cline_code(ss.str())), ss.str()));

                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, susy_dict::observable::bsmumu),
                                   susy_dict::observable::bsmumu + string(" # 4.5e-9"))); // combined LHC limit
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, susy_dict::observable::gmuon),
                                   susy_dict::observable::gmuon + string(" -11.4e-10 9.4e-9"))); // Brookhaven
                break;

            case 'b': // this is if Misiak correction already done

                need_observables = true;

                ss.str("");
                ss.clear();
                ss << "bsgnlo " << (bsgnlo_accepted - bsgnlo_lim) << " " << (bsgnlo_accepted + bsgnlo_lim);
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, get_cline_code(ss.str())), ss.str()));

                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, susy_dict::observable::bsmumu),
                                   susy_dict::observable::bsmumu + string(" # 4.5e-9"))); // combined LHC limit
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::output, susy_dict::observable::gmuon),
                                   susy_dict::observable::gmuon + string(" -11.4e-10 9.4e-9"))); // Brookhaven
                break;

            case 'P':
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(higgs)), higgs)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(neutralino1)), neutralino1)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(chargino1)), chargino1)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(selectronr)), selectronr)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(smuonr)), smuonr)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(stau1)), stau1)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(sbottom1)), sbottom1)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(stop1)), stop1)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::slha, get_cline_code(gluino)), gluino)
                );
                gopts->constraints.push_back(
                        constraint(model_lookup(model_lookup::special, get_cline_code(squark)), squark)
                );

                break;

            case 'l':
            case 'v':
                gopts->print_cuts = true;
                break;

            case 'p':
                gopts->pretend = true;
                break;

            case 'f':
                gopts->force = true;
                break;

            case 'a':
                gopts->append = true;
                break;


            case 0:
            case 'h':
            case '?':
            default:
                return 1;
        }
    }

    if (gopts->merged_input && gopts->obs_file) {
        cerr << "You cannot give both combined RGE/Observable inputs AND a file of observables!" << endl;
        return 1;
    }
    if (need_observables && (!(gopts->merged_input) && !(gopts->obs_file))) {
        cerr << "You specified command line arguments that require observable data, but" << endl
        << "you have not provided any! (Try --merged or --obsfile.)" << endl;
        return 1;
    }
    if (need_likeconfig && !gopts->likeconfig) {
        cerr << "In order to use --chi2, you must provide the likelihood function conf file (--likeconfig)." << endl;
        return 1;
    }

    if (gopts->likeconfig) {
        fstream likeread;
        likeread.open(gopts->likeconfig_filename.c_str(), fstream::in);
        if (!likeread.is_open() || likeread.fail()) {
            cerr << "Unable to open " << gopts->likeconfig_filename << " for reading!" << endl;
            return 1;
        }
        cerr << "Loading likelihood function from " << gopts->likeconfig_filename << "..." << endl;
        hepstats::likeconfig likeconfig(&likeread);
        hepstats::loglike loglike = likeconfig.get_loglike_fn();

        for_each(like_limits.begin(), like_limits.end(),
                 [&gopts, &loglike](const string &lim) -> void {
                     gopts->constraints.push_back(constraint(loglike, lim));
                 }
        );
    }


    int argi = optind;

    if (argi >= argc) {
        gopts->infile = "[stdin]";
        gopts->use_stdin = true;

        gopts->outfile = "[stdout]";
        gopts->use_stdout = true;
    }
    else {
        gopts->infile = argv[argi++];
        gopts->use_stdin = false;
        if (argi >= argc) {
            gopts->outfile = "[stdout]";
            gopts->use_stdout = true;
        }
        else {
            gopts->outfile = argv[argi++];
            gopts->use_stdout = false;
        }
    }

    return 0;
}

#ifdef SUJ_NO_STDOUT
string get_outfile(string infile)
{
    size_t last_slash = infile.find_last_of("/\\");
    size_t last_period = infile.find_last_of(".");
    if (	(last_period != string::npos)					// there is a period
        && (last_slash == string::npos || last_slash < last_period) ) 	// and, there is no slash, or the slash is before the period
    {
        infile.insert(last_period+1,"cons.");
    }
    else
    {
        cout << last_slash << " " << last_period << endl;
        infile.insert(infile.length(),".cons.db");
    }
    return infile;
}
#endif

