
#include "libconstrain.h"

// this class will implement thread-local
// storage to allow parallel calls to FeynHiggs
// ... in the future

class feynhiggs_driver
{
public:
	feynhiggs_driver();

	void operator() (const model &m);

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

	void calc_observables(const model &m);

	void pass_feynhiggs_slha_data(const model &m);

};


