#ifdef __INTEL_COMPILER 			// if the MultiNest library was compiled with ifort
       #define NESTRUN nested_mp_nestrun_
#elif defined __GNUC__ 				// if the MultiNest library was compiled with gfortran
       #define NESTRUN __nested_MOD_nestrun
#else
       #error Do not know how to link to Fortran libraries, check symbol table for your platform (nm libnest3.a | grep nestrun)
#endif

#ifndef MULTINEST_H
#define MULTINEST_H
#pragma once

#include <stdexcept>
#include <string>
#include <memory>

#include "libconstrain.hpp"
#include "libsusykit.hpp"
#include "priors.hpp"

void create_dir(const std::string &dir);
void set_loglike_conf(const std::string &like_conf_name);
std::string get_full_root(const std::string &dir, const std::string &file_root);


// N.B. YOU MUST WRITE THIS FUNCTION!!
extern std::unique_ptr<softsusy_opts> map_cube_to_opts(double *cube);

/***************************************** C++ Interface to MultiNest **************************************************/

namespace nested
{
#ifdef OLD_MN
	extern "C" {
		void NESTRUN(int &mmodal, int &ceff, int &nlive, double &tol, double &efr, int &ndims,
			int &nPar, int &nClsPar, int &maxModes, int &updInt, double &Ztol, char *root, int &seed,
			int *pWrap, int &fb, int &resume, int &outfile, int &initMPI, double &logZero, int &maxiter,
			void (*Loglike)(double *Cube, int &n_dim, int &n_par, double &lnew, void *),
			void (*dumper)(int &, int &, int &, double **, double **, double **, double &, double &, double &, void *),
			void *context, int &root_len);
	}

	static void run(bool mmodal, bool ceff, int nlive, double tol, double efr, int ndims, int nPar, int nClsPar, int maxModes,
		int updInt, double Ztol, const std::string & root, int seed, int *pWrap, bool fb, bool resume, bool outfile, 
		bool initMPI, double logZero, int maxiter, void (*LogLike)(double *Cube, int &n_dim, int &n_par, double &lnew, void *),
		void (*dumper)(int &, int &, int &, double **, double **, double **, double &, double &, double &, void *), void *context)
	{
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
		
		NESTRUN(t_mmodal, t_ceff, nlive, tol, efr, ndims, nPar, nClsPar, maxModes, updInt, Ztol, t_root, seed, pWrap, t_fb, 
		t_resume, t_outfile, t_initMPI, logZero, maxiter, LogLike, dumper, context, root_len);
	}

#else

	extern "C" {
		void NESTRUN(int &IS, int &mmodal, int &ceff, int &nlive, double &tol, double &efr, int &ndims,
			int &nPar, int &nClsPar, int &maxModes, int &updInt, double &Ztol, char *root, int &seed,
			int *pWrap, int &fb, int &resume, int &outfile, int &initMPI, double &logZero, int &maxiter,
			void (*Loglike)(double *Cube, int &n_dim, int &n_par, double &lnew, void *),
			void (*dumper)(int &, int &, int &, double **, double **, double **, double &, double &, double &, void *),
			void *context, int &root_len);
	}

	static void run(bool IS, bool mmodal, bool ceff, int nlive, double tol, double efr, int ndims, int nPar, int nClsPar, int maxModes,
		int updInt, double Ztol, const std::string & root, int seed, int *pWrap, bool fb, bool resume, bool outfile, 
		bool initMPI, double logZero, int maxiter, void (*LogLike)(double *Cube, int &n_dim, int &n_par, double &lnew, void *),
		void (*dumper)(int &, int &, int &, double **, double **, double **, double &, double &, double &, void *), void *context)
	{
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
		int t_IS = IS;
		int t_ceff = ceff;
		
		NESTRUN(t_IS, t_mmodal, t_ceff, nlive, tol, efr, ndims, nPar, nClsPar, maxModes, updInt, Ztol, t_root, seed, pWrap, t_fb, 
		t_resume, t_outfile, t_initMPI, logZero, maxiter, LogLike, dumper, context, root_len);
	}	


#endif
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

void log_like_request(double *Cube, int &ndim, int &npars, double &lnew, void *context);


/************************************************* dumper routine ******************************************************/

// The dumper routine will be called every updInt*10 iterations
// MultiNest doesn not need to the user to do anything. User can use the arguments in whichever way he/she wants
//
//
// Arguments:
//
// nSamples 						= total number of samples in posterior distribution
// nlive 						= total number of live points
// nPar 						= total number of parameters (free + derived)
// physLive[1][nlive * (nPar + 1)] 			= 2D array containing the last set of live points (physical parameters plus derived parameters) along with their loglikelihood values
// posterior[1][nSamples * (nPar + 2)] 			= posterior distribution containing nSamples points. Each sample has nPar parameters (physical + derived) along with the their loglike value & posterior probability
// paramConstr[1][4*nPar]:
// paramConstr[0][0] to paramConstr[0][nPar - 1] 	= mean values of the parameters
// paramConstr[0][nPar] to paramConstr[0][2*nPar - 1] 	= standard deviation of the parameters
// paramConstr[0][nPar*2] to paramConstr[0][3*nPar - 1] = best-fit (maxlike) parameters
// paramConstr[0][nPar*4] to paramConstr[0][4*nPar - 1] = MAP (maximum-a-posteriori) parameters
// maxLogLike						= maximum loglikelihood value
// logZ							= log evidence value
// logZerr						= error on log evidence value
// context						void pointer, any additional information


extern void dumper(int &nSamples, int &nlive, int &nPar, double **physLive, double **posterior, double **paramConstr, double &maxLogLike, double &logZ, double &logZerr, void *context);

/***********************************************************************************************************************/
#endif // MULTINEST_H
