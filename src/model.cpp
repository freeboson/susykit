

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


#include "model.hpp"
#include <sstream>
#include <iterator>

using namespace std;
using namespace susy_dict;

class make_obs_pair
{
public:
	pair<string, double> operator()(double val, const string &key) const
	{
		return pair<string,double> (key,val);
	}
};

model::model(string _model_name, map<string,double> _slha_data, susy_model _model_type)
:model_name(_model_name),slha_data(_slha_data),model_type(_model_type),obs_type(obs_invalid)
{
	build_spectrum();
	build_hierarchy();
	fix_minpar_keys();
	blank_observables();
}

model::model(string _model_name, map<string,double> _slha_data, susy_model _model_type, const string &obs_dat_line)
:model_name(_model_name),slha_data(_slha_data),model_type(_model_type)
{
	build_spectrum();
	build_hierarchy();

	set_observables(obs_dat_line);
	fix_minpar_keys();
}

void model::build_spectrum()
{
	spectrum.clear();

	spectrum.insert(pair<string,double>(m_h0,slha_data[m_h0]));
	spectrum.insert(pair<string,double>(m_H0,slha_data[m_H0]));
	spectrum.insert(pair<string,double>(m_A0,slha_data[m_A0]));
	spectrum.insert(pair<string,double>(m_Hpm,slha_data[m_Hpm]));
	spectrum.insert(pair<string,double>(m_g,slha_data[m_g]));
	spectrum.insert(pair<string,double>(m_o1,slha_data[m_o1]));
	spectrum.insert(pair<string,double>(m_o2,slha_data[m_o2]));
	spectrum.insert(pair<string,double>(m_1pm,slha_data[m_1pm]));
	spectrum.insert(pair<string,double>(m_o3,slha_data[m_o3]));
	spectrum.insert(pair<string,double>(m_o4,slha_data[m_o4]));
	spectrum.insert(pair<string,double>(m_2pm,slha_data[m_2pm]));
	spectrum.insert(pair<string,double>(m_d_l,slha_data[m_d_l]));
	spectrum.insert(pair<string,double>(m_u_l,slha_data[m_u_l]));
	spectrum.insert(pair<string,double>(m_s_l,slha_data[m_s_l]));
	spectrum.insert(pair<string,double>(m_c_l,slha_data[m_c_l]));
	spectrum.insert(pair<string,double>(m_b_1,slha_data[m_b_1]));
	spectrum.insert(pair<string,double>(m_t_1,slha_data[m_t_1]));
	spectrum.insert(pair<string,double>(m_e_l,slha_data[m_e_l]));
	spectrum.insert(pair<string,double>(m_nue_l,slha_data[m_nue_l]));
	spectrum.insert(pair<string,double>(m_mu_l,slha_data[m_mu_l]));
	spectrum.insert(pair<string,double>(m_numu_l,slha_data[m_numu_l]));
	spectrum.insert(pair<string,double>(m_stau_1,slha_data[m_stau_1]));
	spectrum.insert(pair<string,double>(m_nu_tau_l,slha_data[m_nu_tau_l]));
	spectrum.insert(pair<string,double>(m_d_r,slha_data[m_d_r]));
	spectrum.insert(pair<string,double>(m_u_r,slha_data[m_u_r]));
	spectrum.insert(pair<string,double>(m_s_r,slha_data[m_s_r]));
	spectrum.insert(pair<string,double>(m_c_r,slha_data[m_c_r]));
	spectrum.insert(pair<string,double>(m_b_2,slha_data[m_b_2]));
	spectrum.insert(pair<string,double>(m_t_2,slha_data[m_t_2]));
	spectrum.insert(pair<string,double>(m_e_r,slha_data[m_e_r]));
	spectrum.insert(pair<string,double>(m_mu_r,slha_data[m_mu_r]));
	spectrum.insert(pair<string,double>(m_stau_2,slha_data[m_stau_2]));
}

void model::build_hierarchy()
{
	// we include in the hierarchy the higgs particles other than the little higgs.
	// i.e., all r-odd particles, and the r-even higgs particles EXCEPT for the 
	// SM-like light h0 particle
	for (map<string,double>::iterator it = spectrum.begin(); it != spectrum.end(); it++)
	{
		if (it->first != m_h0)
			hierarchy.insert(pair<double,string>(abs(it->second),it->first));
	}
}

void model::fix_minpar_keys()
{
	if (model_type == mSUGRA)
	{
		return;
	}
	if (model_name == "NUSUGRA")
		return;
	istringstream nu_flag_parse(model_name);
	string nu_base;
	string nu_flags;
	if (!getline(nu_flag_parse,nu_base,'-') || nu_base != "NUSUGRA" || !getline(nu_flag_parse,nu_flags,'-'))
	{
		cerr << "malformed model_name (" << model_name << ")!" << endl;
		return;
	}
	// here we know that model_name was of the form NUSUGRA-<something>
	// where <something> has been stored in nu_flags
	if (string::npos == nu_flags.find('G'))
	{
		slha_data.insert(pair<string,double>(mhf,slha_data[m1_X])); //TODO: verify m1=m2=m3:=mhf
	}
	if (string::npos == nu_flags.find('A'))
	{
		slha_data.insert(pair<string,double>(a0,slha_data[at_X])); //TODO: verify at=ab=atau:=a0
	}
	if (true)  // TODO: come up with a way of figuring out m0
	{
		slha_data.insert(pair<string,double>(m0,slha_data[m_e_l_X]));
	}
}

double model::get_datum(const string &datum_key) const
{
	map<string,double>::const_iterator it = slha_data.find(datum_key);
	if (it == slha_data.end())
	{
		cerr << datum_key << " is not a valid datum key! (Check dict.h.)" << endl;
		return 0;
	}
	return it->second;
}

pair<std::string,double> model::get_mass_pair(const string &pname) const
{
	map<string,double>::const_iterator it = spectrum.find(pname);
	cerr << pname << " is not a particle mass in the spectrum!" << endl;
	return *it;
}

string model::get_hierarchy(size_t n) const
{
	multimap<double,string>::const_iterator it = hierarchy.begin();
	if (n >= hierarchy.size())
	{
		cerr << "Error: Hierarchy position out of range. pos=" << n << " size=" << hierarchy.size() << endl;
		return "BEYOND_LIMT";
	}
	advance(it,n);
	return it->second;
}

bool model::hierarchy_cmp(const model &m, const int &n) const
{
	if (hierarchy.size() < n || m.hierarchy.size() < n) return true;

	multimap<double,string>::const_iterator it1 = hierarchy.begin();
	multimap<double,string>::const_iterator it2 = m.hierarchy.begin();

	multimap<double,string>::const_iterator it_end = hierarchy.begin();
	advance(it_end,n);

	while (it1 != it_end)
	{
		if (it1->second != it2->second)
			return true;
		++it1;
		++it2;
	}
	return false;
}

string model::get_slha() const
{
	return "NOT READY YET!";
}

void model::write_slha(std::ostream &out) const
{
	out << get_slha() << endl;
}

void model::set_observables(const std::string &obs_dat_line)
{
	stringstream iss(obs_dat_line);

	int valid_bit;
	if (iss >> valid_bit)
	{
		unsigned int num_data = distance(istream_iterator<double>(iss), 
					istream_iterator<double>());
		if (1 != valid_bit)
		{
			cerr << "valid bit=" << valid_bit << " was not set!!" << endl;
			obs_type = obs_invalid;
			blank_observables();
			return;
		}
		if ((num_data + 1) != observable::observe_row.size() || num_data < 1) // +1 to account for the "valid_bit"
		{
			cerr << "Error: malformed observables data line: " 
			     << num_data << " doubles provided" 
			     << endl;
			obs_type = obs_invalid;
			blank_observables();
		}
		else
		{
			iss.clear();
			iss.seekg(ios_base::beg);

			transform (	istream_iterator<double> (iss),
					istream_iterator<double> (),
					observable::observe_row.begin(),
					inserter(observables, observables.begin()),
					make_obs_pair()
			);
			obs_type = valid;
		}
	}
	else
	{
		cerr << "Couldn't even check valid bit!" << endl;
		obs_type = obs_invalid;
		blank_observables();
	}

}

double model::get_observable(const std::string &obs_datum_key) const
{
	map<string,double>::const_iterator it = observables.find(obs_datum_key);
	if (it == observables.end())
	{
		cerr << obs_datum_key << " is not a valid datum key! (Check dict.h.)" << endl;
		return 0;
	}
	return it->second;
}

ostream& operator<< (ostream &o, const pair<string,double> &p)
{
	o << "(" << p.first << "," << p.second << ")";
	return o;
}

ostream& operator<< (ostream &o, const pair<double,string> &p)
{
	o << "(" << p.second << "," << p.first << ")";
	return o;
}

void model::blank_observables()
{
	for (const auto &observable_key : observable::observe_row)
		observables[observable_key] = 0.0;
}

