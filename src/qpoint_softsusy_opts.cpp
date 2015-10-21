
#include "qpoint_softsusy_opts.hpp"

#include "softsusy.h" // for the bc's

#include <unistd.h>
#include <getopt.h>

#include <iostream>
#include <iomanip>
#include <list>
#include <exception>

using namespace std;

qpoint_opts::qpoint_opts(int argc, char**argv)
:softsusy_opts(),slha_format(false)
{
	try {
		opthandle(argc,argv);

	} catch (invalid_argument &e) {
		cerr << e.what() << endl;

		usage();

		throw;
	}

}

void qpoint_opts::usage() const
{
	cerr 	<< setiosflags(ios::left)
		<< "General command:" << endl
		<< "qpoint [options] -- <m0> [MH1 MH2] [mq ml] [m3rd] <MG1 MG2 MG3 | mhf> <At Ab Atau | A0> <tb> <sgn(mu)>" << endl
		<< endl
		<< "The following options are available:" << endl
		<< "  " << setw(26) << "-H, --help" << " print this message" << endl
		<< "  " << setw(26) << "-s, --slha" << " output SLHA to stderr" << endl
		<< endl
		<< "Non-universality flags:" << endl
		<< "  " << setw(26) << "-h, --higgs" << " seperate masses for Higgs fields, requires MH1 and MH2" << endl
		<< "  " << setw(26) << "-g, --gaugino" << "    ... for gaugino fields, requires MG1, MG2, and MG3 instead of mhf" << endl
		<< "  " << setw(26) << "-t, --trilinear" << "    ... for trilinear couplings, requires At, Ab, and Atau instead of A0" << endl
		<< "  " << setw(26) << "-3, --3rd-gen" << "    ... for 3rd gen sfermions, requires m3rd" << endl
		<< "  " << setw(26) << "-f, --mq-ml" << "    ... for squarks and sleptons, requires mq and ml" << endl
		<< "  " << setw(26) << "-F, --full-sugra" << " all masses and couplings are specified" << endl
		<< endl
		<< "Optional nuisance parameters:" << endl
		<< "  " << setw(26) << "    --alpha-s=VAL" << " alpha(strong), at scale MZ, by default:" << default_alpha_s << endl
		<< "  " << setw(26) << "    --alpha-em-inv=VAL" << " 1/alpha(EM), at scale MZ, by default:" << default_alpha_em_inv << endl
		<< "  " << setw(26) << "    --mtop=VAL" << " top quark pole mass, by default:" << default_mtop << endl
		<< "  " << setw(26) << "    --mbmb=VAL" << " running bottom quark mass mb(mb), by default:" << default_mbmb << endl
		<< endl;
}

int qpoint_opts::opthandle(int argc, char** argv)
{
	static option longopts[] = {
		// help
		{"help",	no_argument,		NULL, 'H'},
		{"slha",	no_argument,		NULL, 's'},
		{"higgs",	no_argument,		NULL, 'h'},
		{"gaugino",	no_argument,		NULL, 'g'},
		{"trilinear",	no_argument,		NULL, 't'},
		{"3rdgen",	no_argument,		NULL, '3'},
		{"3rd-gen",	no_argument,		NULL, '3'},
		{"mqml",	no_argument,		NULL, 'f'},
		{"mq-ml",	no_argument,		NULL, 'f'},
		{"full",	no_argument,		NULL, 'F'},
		{"full-sugra",	no_argument,		NULL, 'F'},
		{"alphas",	required_argument,	NULL, 'S'},
		{"alpha-s",	required_argument,	NULL, 'S'},
		{"alphaem",	required_argument,	NULL, 'e'},
		{"alpha-em",	required_argument,	NULL, 'e'},
		{"alphaeminv",	required_argument,	NULL, 'e'},
		{"alpha-eminv",	required_argument,	NULL, 'e'},
		{"alpha-em-inv",required_argument,	NULL, 'e'},
		{"mtop",	required_argument,	NULL, 'T'},
		{"mb",		required_argument,	NULL, 'b'},
		{"mbmb",	required_argument,	NULL, 'b'},
		{ NULL,		no_argument,		NULL, 0}
	};

	static const char *optcstr = "Hshgt3fFS:e:T:b:";

	int longindex = 0;

	int opt;

	unsigned int num_required_args = 5; // base case is mSUGRA 4 + sign(mu)
	bool need_help = false;

	while (-1 != (opt=getopt_long(argc, argv, optcstr, longopts, &longindex)))
	{
		try {
			switch(opt)
			{
	
				case 'h':
					higgs = true;
					num_required_args += 2; // 2 extra for mHu mHd
					break;
	
				case 'g':
					gaugino = true;
					num_required_args += 2; // mhf -> M1, M2, M3 => 2 extra 
					break;
	
				case 't':
					trilinear = true;
					num_required_args += 2; // a0 -> at, ab, atau => 2 extra
					break;

#if 0	// this would mean that each 3rd gen scalar gets a different mass [wrong]

				case '3':
					thirdgen = true;
					num_required_args += 5; // 5 extra for m_ql3, m_t_r, m_b_r, m_stau_l, m_stau_r
					break;

#else   // this would mean that all 3rd gen scalars get a specific mass [right]

				case '3':
					thirdgen = true;
					num_required_args += 1; // 1 extra for m3rd
					break;
#endif

				case 'f':
					mq_ml = true;
					num_required_args += 2; // 2 extra for mq, ml
								// if used with --higgs, m0 is not necessary
					break;
	
				case 'F':
					full_sugra = true;	// this is a special case -- deal with it later
					break;
				
				case 'S':
					if (NULL == optarg) 
						throw(runtime_error("getopt_long() error!!!"));

					alpha_s = stod(optarg);
					break;

				case 'e':
					if (NULL == optarg)
						throw(runtime_error("getopt_long() error!!!"));
					alpha_em_inv = stod(optarg);
					break;

				case 'T':
					if (NULL == optarg)
						throw(runtime_error("getopt_long() error!!!"));
					mtop_pole = stod(optarg);
					break;
	

				case 'b':
					if (NULL == optarg)
						throw(runtime_error("getopt_long() error!!!"));
					mbmb = stod(optarg);
					break;

				case 's':
					slha_format = true;
					break;

				case 0:
				case ':':
				case 'H':
				case '?':
				default:
					need_help = true;
					break;
			}
		} catch (exception const &e) {
			throw(invalid_argument("Error: Malformed input"));
		}
	}

	nusugra = (higgs || gaugino || trilinear || thirdgen || mq_ml);

	// checks on the arguments
	if (higgs && mq_ml)
	{
		num_required_args--; // m0 no longer needed
	}

	if (full_sugra)
	{
		if (nusugra)
			throw(invalid_argument("Error: Do not combine --full-sugra with any non-universal flags"));

		nusugra = true;
		num_required_args = (3 + 3 + 2 + 6 + 9) + 1 + 1; // ( gauginos + trilinear 
								 //  + Higgses + sleptons
								 //  + squarks ) + tb + sgn(mu)
	}

	if (need_help)
		throw(invalid_argument(""));

	int num_remaining_args = argc - optind; // optind is declared in getopt.h
		   			        // it is equal to the "next" index in argv 
			   			// after processing the getopt()'s

	if ( (num_remaining_args <= 0) || (num_remaining_args != num_required_args) )
		throw( invalid_argument("Error: expected " + to_string(num_required_args) 
		+ " arguments but only found " + to_string(num_remaining_args) + ".")	   );

	try {

		make_pars(optind, argv);

	} catch (exception const &e) {

		throw(invalid_argument("Error: Malformed input"));
	}

	return 0;
}

void qpoint_opts::make_pars(int argi, char** argv)
{

	double m0, mhf, a0;
	double m_h1, m_h2;
	double mq, ml;
	double m_3rd;
	double m1, m2, m3;
	double at, ab, atau;


	if ( (!higgs || !mq_ml) && !full_sugra )
	{
		m0 = stod(argv[argi++]);
	}

	if (higgs && !full_sugra)
	{
		m_h1 = stod(argv[argi++]);
		m_h2 = stod(argv[argi++]);
	}

	if (mq_ml)
	{
		mq = stod(argv[argi++]);
		ml = stod(argv[argi++]);
	}

	if (thirdgen)
	{
		m_3rd = stod(argv[argi++]);
	}

	if (gaugino)
	{
		m1 = stod(argv[argi++]);
		m2 = stod(argv[argi++]);
		m3 = stod(argv[argi++]);
	}
	else
	{
		mhf = stod(argv[argi++]);
	}

	if (trilinear)
	{
		at = stod(argv[argi++]);
		ab = stod(argv[argi++]);
		atau = stod(argv[argi++]);
	}
	else
	{
		a0 = stod(argv[argi++]);
	}

	tb = stod(argv[argi++]);
	sgnmu = stoi(argv[argi++]);

	// all args have been extracted; all params set
	// now populate pars for SOFTSUSY
	

	if (!nusugra)
	{
		pars.setEnd(3); // mSUGRA pars: m0, mhf, a0

		pars(1) = m0;
		pars(2) = mhf;
		pars(3) = a0;

//		cerr << "mSUGRA! m0=" << pars(1) << " mhf=" << mhf << " a0=" << a0 << " tb=" << tb << " sgnmu=" << sgnmu <<endl;

	}
	else 
	{
		pars.setEnd(49); // NUSUGRA pars: not actually 49, but that's the index of the last one

		// gaugino masses
		if (gaugino)
		{
			pars(1) = m1;
			pars(2) = m2;
			pars(3) = m3;
		}
		else
		{
			pars(1) = mhf;
			pars(2) = mhf;
			pars(3) = mhf;
		}

		// trilinear couplings
		if (trilinear)
		{
			pars(11) = at;
			pars(12) = ab;
			pars(13) = atau;
		}
		else
		{
			pars(11) = a0;
			pars(12) = a0;
			pars(13) = a0;
		}

		// Higgs field squared-masses
		if (higgs)
		{
			pars(21) = m_h1*abs(m_h1);
			pars(22) = m_h2*abs(m_h2);
		}
		else
		{
			pars(21) = m0*abs(m0);
			pars(22) = m0*abs(m0);
		}

		// scalar leptons
		if (mq_ml)
		{
			for (int slepton = 31; slepton <= 36; slepton++)
				pars(slepton) = ml;

			// scalar quarks
			for (int squark = 41; squark <= 49; squark++)
				pars(squark) = mq;
		}
		else
		{
			for (int slepton = 31; slepton <= 36; slepton++)
				pars(slepton) = m0;

			// scalar quarks
			for (int squark = 41; squark <= 49; squark++)
				pars(squark) = m0;
		}

		if (thirdgen)
		{
			// in order, stau_l, stau_r, ql3, t_r, b_r
			const list<unsigned int> thirdgen_pars = { 33, 36, 43, 46, 49 };
			for (const unsigned int &index : thirdgen_pars)
				pars(index) = m_3rd;
		}
	}
}


