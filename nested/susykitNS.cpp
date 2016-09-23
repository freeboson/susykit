/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - susykitNS.cpp:                                                           -
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


#include <iostream>
#include <array>
#include <cstring>
#include <forward_list>

#include "nestface.hpp"
#include "hepstats.hpp"
#include "priors.hpp"

const double logZero = -1e90;

namespace multinest_options {
    int IS = 1;                    // do importance sampling?
    int mmodal = 1;                    // do mode separation?
    int ceff = 0;                    // run in constant efficiency mode?
    int nlive = 20000;                // number of live points
    double efr = 0.8;                // set the required efficiency
    double tol = 1e-4;                // tol, defines the stopping criteria
    const int ndims = 10;                // dimensionality (no. of free parameters)
    int nPar = ndims + (susy_dict::NUSUGRA_row.size() - 1) +
               susy_dict::observable::observe_row.size();    // total no. of parameters including free & derived parameters
    int nClsPar = 6;                // no. of parameters to do mode separation on
    int updInt = 2000;                // after how many iterations feedback is required & the output files should be updated
    // note: posterior files are updated & dumper routine is called after every updInt*10 iterations
    double Ztol = logZero;                // all the modes with logZ < Ztol are ignored
    int maxModes = 100;                // expected max no. of modes (used only for memory allocation)
    int pWrap[ndims];                // which parameters to have periodic boundary conditions?
    char root[100];
    int seed = -1;                    // random no. generator seed, if < 0 then take the seed from system clock
    int fb = 1;                    // need feedback on standard output?
    int resume = 1;                    // resume from a previous job?
    int outfile = 1;                // write output files?
    int initMPI = 1;                // initialize MPI routines?, relevant only if compiling with MPI
    // set it to F if you want your main program to handle MPI initialization
    double logzero = logZero;            // points with loglike < logZero will be ignored by MultiNest
    int maxiter = 0;                // max no. of iterations, a non-positive value means infinity. MultiNest will terminate if either it
    // has done max no. of iterations or convergence criterion (defined through tol) has been satisfied
    void *context = 0;                // not required by MultiNest, any additional information user wants to pass
}

std::array<std::shared_ptr<priors::prior>, multinest_options::ndims> prior_list;

std::unique_ptr<softsusy_opts> map_cube_to_opts(double *cube);

void init_priors();

int main(int argc, char **argv) {

    std::string out_dir, out_root, like_conf_name;

    out_dir = std::string(argv[1]);
    out_root = std::string(argv[2]);
    like_conf_name = std::string(argv[3]);

    std::string full_root;

    try {

        init_priors();
        create_dir(out_dir);
        set_loglike_conf(like_conf_name);
        full_root = get_full_root(out_dir, out_root);

    } catch (std::exception &e) {

        std::cerr << "Caught an exception!" << endl
        << e.what() << endl;

        return 1;
    }


    for (int i = 0; i < multinest_options::ndims; i++) multinest_options::pWrap[i] = 0;
    strcpy(multinest_options::root, full_root.c_str());

    std::forward_list<int> inputs = {31, 1, 2, 3, 11}; // m0, m1, m2, m3, a0

    multinest_options::context = static_cast<void *>(&inputs);

    nested::run(
            multinest_options::IS,
            multinest_options::mmodal,
            multinest_options::ceff,
            multinest_options::nlive,
            multinest_options::tol,
            multinest_options::efr,
            multinest_options::ndims,
            multinest_options::nPar,
            multinest_options::nClsPar,
            multinest_options::maxModes,
            multinest_options::updInt,
            multinest_options::Ztol,
            multinest_options::root,
            multinest_options::seed,
            multinest_options::pWrap,
            multinest_options::fb,
            multinest_options::resume,
            multinest_options::outfile,
            multinest_options::initMPI,
            multinest_options::logzero,
            multinest_options::maxiter,
            log_like_request,
            dumper,
            multinest_options::context
    );

    return 0;
}

void dumper(int &nSamples, int &nlive, int &nPar, double **physLive, double **posterior, double **paramConstr,
            double &maxLogLike, double &logZ, double &logZerr, void *context) {
    return;
}

void init_priors() {
    prior_list = {
            make_shared<priors::log>(1.0, 3.0), // m0  // these will be < 1 TeV
            make_shared<priors::log>(1.0, 3.0), // m1
            make_shared<priors::log>(1.0, 3.0), // m2
            make_shared<priors::log>(2.7, 4.7), // m3 // start gluino mass > 500 GeV
            make_shared<priors::linear>(-50e3, 50e3), // a0
            make_shared<priors::linear>(2.0, 62.0), // tb

            make_shared<priors::linear>(173.07 - 2 * 0.789, 173.07 + 2 * 0.789), // mtop(pole)
            make_shared<priors::linear>(4.18 - 2 * 0.03, 4.18 + 2 * 0.03), // mb(mb)
            make_shared<priors::linear>(0.1184 - 2 * 7e-4, 0.1184 + 2 * 7e-4), // alpha_s(MZ)
            make_shared<priors::linear>(127.916 - 2 * 0.015, 127.916 + 2 * 0.015), // alpha_em_inv(MZ)
    };
}

// user-defined (here) (now) (...)
std::unique_ptr<softsusy_opts> map_cube_to_opts(double *cube) {

    // scale the inputs
    std::transform(
            prior_list.begin(), // input1 (start)
            prior_list.end(),   // input1 (end)
            cube,            // input2
            cube,            // output

            // binary operation, as lambda fn
            [](std::shared_ptr<priors::prior> prior, double variate) -> double {
                return prior->map(variate);
            }
    );

    // ok, we have scaled the inputs, now we have to build the softsusy driver options

    std::unique_ptr<softsusy_opts> point(new softsusy_opts()); // populate this

    point->set_sgnmu(1);

    // extract cube data to meaningful labels

    std::size_t cube_index = 0;

    // main params
    const double *const m0 = &cube[cube_index++];
    const double *const m1 = &cube[cube_index++];
    const double *const m2 = &cube[cube_index++];
    const double *const m3 = &cube[cube_index++];
    const double *const a0 = &cube[cube_index++];
    const double *const tb = &cube[cube_index++];

    // nuisance params
    const double *const mtop = &cube[cube_index++];
    const double *const mbmb = &cube[cube_index++];
    const double *const alpha_s = &cube[cube_index++];
    const double *const alpha_em_inv = &cube[cube_index++];

    // build pars
    DoubleVector pars(49);

    // gauginos
    pars(1) = *m1;
    pars(2) = *m2;
    pars(3) = *m3;

    // trilinear couplings
    pars(11) = *a0;
    pars(12) = *a0;
    pars(13) = *a0;

    // Higgs field mass^2
    pars(21) = (*m0) * abs(*m0);
    pars(22) = (*m0) * abs(*m0);

    // sfermions
    for (int slepton = 31; slepton <= 36; slepton++)
        pars(slepton) = *m0;

    for (int squark = 41; squark <= 49; squark++)
        pars(squark) = *m0;

    point->set_pars(pars);
    point->set_tb(*tb);
    point->set_mtop_pole(*mtop);
    point->set_mbmb(*mbmb);
    point->set_alpha_s(*alpha_s);
    point->set_alpha_em_inv(*alpha_em_inv);

    return point;
}


