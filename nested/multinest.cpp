/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - multinest.cpp:                                                           -
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


#ifdef __INTEL_COMPILER            // if the MultiNest library was compiled with ifort
#define NESTRUN nested_mp_nestrun_
#elif defined __GNUC__ 				// if the MultiNest library was compiled with gfortran
#define NESTRUN __nested_MOD_nestrun
#else
#error Do not know how to link to Fortran libraries, check symbol table for your platform (nm libnest3.a | grep nestrun)
#endif

#ifndef MULTINEST_H
#define MULTINEST_H

/***************************************** C++ Interface to MultiNest **************************************************/

namespace nested {
    extern "C" {
    void NESTRUN(int &mmodal, int &ceff, int &nlive, double &tol, double &efr, int &ndims,
                 int &nPar, int &nClsPar, int &maxModes, int &updInt, double &Ztol, char *root, int &seed,
                 int *pWrap, int &fb, int &resume, int &outfile, int &initMPI, double &logZero, int &maxiter,
                 void (*Loglike)(double *Cube, int &n_dim, int &n_par, double &lnew, void *),
                 void (*dumper)(int &, int &, int &, double **, double **, double **, double &, double &, double &,
                                void *),
                 void *context, int &root_len);
    }

    static void run(bool mmodal, bool ceff, int nlive, double tol, double efr, int ndims, int nPar, int nClsPar,
                    int maxModes,
                    int updInt, double Ztol, const std::string &root, int seed, int *pWrap, bool fb, bool resume,
                    bool outfile,
                    bool initMPI, double logZero, int maxiter,
                    void (*LogLike)(double *Cube, int &n_dim, int &n_par, double &lnew, void *),
                    void (*dumper)(int &, int &, int &, double **, double **, double **, double &, double &, double &,
                                   void *), void *context) {
        char t_root[100];
        std::fill(t_root, t_root + 100, ' ');
        snprintf(t_root, 99, "%s", root.c_str());
        int root_len = strlen(t_root);
        t_root[strlen(t_root)] = ' ';

        int t_fb = fb;
        int t_resume = resume;
        int t_outfile = outfile;
        int t_initMPI = initMPI;
        int t_mmodal = mmodal;
        int t_ceff = ceff;

        NESTRUN(t_mmodal, t_ceff, nlive, tol, efr, ndims, nPar, nClsPar, maxModes, updInt, Ztol, t_root, seed, pWrap,
                t_fb,
                t_resume, t_outfile, t_initMPI, logZero, maxiter, LogLike, dumper, context, root_len);
    }

}

/***********************************************************************************************************************/

#endif // MULTINEST_H
