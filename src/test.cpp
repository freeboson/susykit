
#include <complex.h>

#if __cplusplus >= 201103L
# define complex _Complex
#endif

extern "C" {

struct parameters
/* structure containing all the scanned parameters from the SLHA file */
{
	double complex Vud,Vus,Vub,Vcd,Vcs,Vcb,Vtd,Vts,Vtb;
};

}
#if __cplusplus >= 201103L
# undef complex
#endif


