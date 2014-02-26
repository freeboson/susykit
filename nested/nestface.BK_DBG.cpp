
#include "nestface.h"

// for create_dir stuff
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// errno stuff
#include <cerrno>
#include <cstring> // for strerror()
#include <sstream>

#include <iostream>
#include <fstream>
#include <sstream>

#include <forward_list>

#include "sujsoft.hpp"
#include "sujmicro.h"
#include "sujfeyn.hpp"

hepstats::loglike loglike_calc;

void create_dir(const std::string &dir)
{
	if (0 != mkdir(dir.c_str(),S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
	{
		if (EEXIST == errno)
		{
			std::cerr << dir << " exists... continuing..." << std::endl;
		}
		else
		{
			std::stringstream error_msg;
			error_msg << "Error creating " << dir << ": " << strerror(errno);
			throw(runtime_error(error_msg.str()));
		}
	}
	else
	{
		std::cerr << "Created directory " << dir << std::endl;
	}
}


void set_loglike_conf(const std::string &like_conf_name)
{
	std::cerr << "Reading likelihood function configuration from " << like_conf_name << "..." << std::endl;

	std::ifstream reader;
	reader.open(like_conf_name.c_str());
	if (reader.fail())
	{
		std::stringstream error_msg;
		error_msg << "Error opening " << like_conf_name << " for reading!";
		throw(runtime_error(error_msg.str()));
	}

	hepstats::likeconfig config(&reader);
	loglike_calc = config();

	if (reader.is_open()) 
		reader.close();
}

std::string get_full_root(const std::string &dir, const std::string &file_root)
{
	std::stringstream full_root;

	full_root << dir << "/" << file_root;
	if (full_root.str().size() > 100)
	{
		throw length_error("Error: director+root larger than arbitrary cap. Feel free to change this.");
	}
	return full_root.str();
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

#define DEBUG
void log_like_request(double *Cube, int &ndim, int &npars, double &lnew, void *context)
{

	model m;
	lnew = -1e90;

	auto sugra = map_cube_to_opts(Cube);

	softsusy_driver softsusy(sugra.get()); // softsusy_driver will just copy the contents
	micromegas_driver micro;
	feynhiggs_driver feynhiggs;
//	superiso_driver superiso; // not ready yet

#ifdef DEBUG

	std::forward_list<int> *pars_inputs = static_cast<std::forward_list<int>*>(context);

	std::stringstream debug_str;

	debug_str << "inputs = [ ";
	for (int index : *pars_inputs)
		debug_str << sugra->pars_at(index) << " ";
	debug_str << sugra->get_tb() << " ]";

#endif


	try { 

		m = softsusy(); // need to check for displayProblem().test() and neutralino LSP 

	} catch (const string &s) { 
#ifdef DEBUG
		debug_str << " :: " << s;
		std::cerr << debug_str.str() << std::endl;
#endif
		return; 
	}

	if (susy_dict::m_o1 != m.get_hierarchy(0))
	{
#ifdef DEBUG
		debug_str << " :: Bad LSP: " << m.get_hierarchy(0);
		std::cerr << debug_str.str() << std::endl;
#endif
		return;	   // neutralino1 is not the LSP...
			  // may use a tolerance in the 
			  // mass gap in the future
	}

	double softsusy_higgs = m.get_datum(susy_dict::m_h0);

	try {

		micro(&m);
		feynhiggs(&m);

	} catch (exception &e) {

		std::cerr << "Observables error :: " << e.what() << std::endl;
		return;

	}

#ifdef DEBUG
		debug_str << " :: Processing likelihood...";
		std::cerr << debug_str.str() << std::endl;
#endif

	lnew = loglike_calc.get_log_like(m);

#ifndef DEBUG
	std::forward_list<int> *pars_inputs = static_cast<std::forward_list<int>*>(context);
#endif

	std::stringstream point_summary;
	point_summary << "ln(like) = " << lnew << " :: ";
	
	point_summary << "inputs = [ ";
	for (int index : *pars_inputs)
		point_summary << sugra->pars_at(index) << " ";
	point_summary << sugra->get_tb() << " ]";

	get_lsl lsl;

	point_summary << " :: outputs = ["
#ifdef DEBUG
		      << " Old[m_h0]= " << softsusy_higgs
#endif
		      << " m_h0= " << m.get_datum(susy_dict::m_h0)
		      << " omega= " << m.get_observable(susy_dict::observable::omega)
		      << " m_o1= " << m.get_datum(susy_dict::m_o1)
		      << " m_1pm= " << m.get_datum(susy_dict::m_1pm)
		      << " m_o2= " << m.get_datum(susy_dict::m_o2)
		      << " m_lsl= " << lsl(m)
		      << " ]";

	std::cerr << point_summary.str() << std::endl;

	auto *slha_row = &susy_dict::mSUGRA_row;
	if (sugra->is_nusugra())
		slha_row = &susy_dict::NUSUGRA_row;

	auto row_begin = slha_row->begin();

	std::transform(
		++row_begin, // skip the model name
		slha_row->end(),
		Cube+ndim, 			 // these start after free params
		[&m] (const std::string &key) -> double {
			return m.get_datum(key);
		}
	);

	std::transform(
		susy_dict::observable::observe_row.begin(),
		susy_dict::observable::observe_row.end(),
		Cube + ndim + (slha_row->size()-1), 
		[&m] (const std::string &key) -> double {
			return m.get_observable(key);
		}
	);

}


