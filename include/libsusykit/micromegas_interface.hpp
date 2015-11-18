

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

#include "libconstrain/dict.hpp"
#include "libconstrain/model.hpp"
#include "libconstrain/model_lookup.hpp"
#include "libconstrain/parse.hpp"

// this class will implement thread-local
// storage to allow parallel calls to micrOMEGAs
// ... in the future

class micromegas_driver
{
public:
	enum relic_mode
	{
		normal, init
	};

	micromegas_driver() :relic_mode_setting(normal) {}
	micromegas_driver(relic_mode _rm)
		:relic_mode_setting(_rm) {}

	model operator() (model m);
	int operator() (model *m);

private:

	void calc_observables(model *m);
	void pass_micromegas_slha_data(const model &m);

	relic_mode relic_mode_setting;
};


