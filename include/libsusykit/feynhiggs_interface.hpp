

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
#ifndef SUJFEYN_HPP
#define SUJFEYN_HPP

#include "libconstrain/dict.hpp"
#include "libconstrain/model.hpp"
#include "libconstrain/model_lookup.hpp"
#include "libconstrain/parse.hpp"
#include "src/include/ftypes.h"

// this class will implement thread-local
// storage to allow parallel calls to FeynHiggs
// ... in the future

#define FH_SLHAData_len 5558

class feynhiggs_driver
{
public:
	feynhiggs_driver();

	void operator() (model *m);

//	~feynhiggs_driver();

	static constexpr unsigned int slhadata_len = FH_SLHAData_len;
private:
	int mssm_scope; 
	int field_renorm; 
	int tanbeta_renorm;
	int higgs_mixing;
	int p2_approx;
	int loop_level;
	int run_mt;
	int bottom_resum;
	int two_loop_complex_approx;
	int debug_level;

	int fh_error_state;

	COMPLEX slha[slhadata_len]; 	// COMPLEX is defined in ftypes.h

	void calc_observables(model *m);

	void pass_feynhiggs_slha_data(const model *m);
};

#endif


