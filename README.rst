::

          ,gg,                                ,ggg,        gg              
         i8""8i                              dP""Y8b       dP         I8   
         `8,,8'                              Yb, `88      d8'         I8   
          `88'                                `"  88    ,dP'    gg 88888888
          dP"8,                                   88aaad8"      ""    I8   
         dP' `8a  gg      gg    ,g,     gg     gg 88""""Yb,     gg    I8   
        dP'   `Yb I8      8I   ,8'8,    I8     8I 88     "8b    88    I8   
    _ ,dP'     I8 I8,    ,8I  ,8'  Yb   I8,   ,8I 88      `8i   88   ,I8,  
    "888,,____,dP,d8b,  ,d8b,,8'_   8) ,d8b, ,d8I 88       Yb,_,88,_,d88b, 
    a8P"Y88888P" 8P'"Y88P"`Y8P' "YY8P8PP""Y88P"88888        Y88P""Y88P""Y8 
                                             ,d8I'                         
                                           ,dP'8I                          
                                          ,8"  8I                          
                                          I8   8I                          
                                          `8, ,8I                          
                                           `Y8P"                           

What is ``SusyKit``?
--------------------
Essentially, ``SusyKit`` is a program that interfaces several packages that are
useful for sampling and calculating the properties of parameter points in
high-scale MSSM-based models.

The packages incorporated in ``SusyKit`` are:
  - `MultiNest <https://ccpforge.cse.rl.ac.uk/gf/project/multinest>`_ provides
    an MPI-based sampling algorithm
  - `SOFTSUSY <http://softsusy.hepforge.org/>`_ performs the RGE evolution and
    loop corrected mass calculations of the sparticles
  - `FeynHiggs
    <http://wwwth.mpp.mpg.de/members/heinemey/feynhiggs/cFeynHiggs.html>`_
    computes masses, couplings, and decays in the Higgs sector to a far greater
    accuracy
  - `DarkSUSY <http://www.darksusy.org/>`_ includes routines for computing an
    extensive variety of physical observables related to dark matter experiments
  - `HiggsBounds <http://higgsbounds.hepforge.org>`_ tests against the 95% CL
    exclusion limits from collider searches in the Higgs sector
  - `SuperIso Relic <http://superiso.in2p3.fr/relic/>`_ is a C library that
    calculates a vast array of flavor physics observables, as well as the
    neutralino thermal relic density considering all tree-level coannihilation
    diagrams
  - `MicrOMEGAs <https://lapth.cnrs.fr/micromegas/>`_ computes the neutralino
    thermal relic density, direct and indirect detection rates for neutralino
    dark matter, along with the same for other dark matter models

``SusyKit`` allows you to sample from the parameter space using prior
distributions and a likelihood function provided to ``MultiNest``, but also
provides a generic MPI-based scanning system that draws purely random samples
from prior distributions.

``SusyKit`` also provides several useful command line utilities for
investigating the parameter space: ``qpoint``, ``constrain`` and ``qparse``.

Installation
------------
``SusyKit`` is *meant* to be simple to install, but as you can probably expect
from a package with inter-language libraries, it gets tricky. The basic
instructions are as follows::

$ git clone git@github.com:freeboson/susykit.git
$ cd susykit
$ cmake .
$ make

That should fetch/unpack/compile/install all of the external libs *and* build
the basic targets: ``qpoint``, ``constrain`` and ``qparse``.

On OS X, it's a bit tricky. To get the proper C++11 support, you have
``clang++`` which is great. However, on OS X 10.8.5 and below, the default is to
use ``libstdc++`` and not ``libc++``, and unfortunately ``cmake`` cannot figure
that out for you. In this case, you will want to use GNU GCC via homebrew. Then
you can run cmake this way::

$ CC=gcc-5 CXX=g++-5

Then, ``cmake`` will use GNU's ``gcc`` and ``g++``. (The ones in ``/usr/bin``
are just symlinks to Apple LLVM.) Note that if you are on **OS X 10.9 or newer**
you do not have to worry about this at all.

``qpoint``
----------
The ``qpoint`` utility allows you to process a parameter point through the
toolchain to compute the sparticle spectrum and physical observables. The output
is not intended to be human readable, but instead should be piped into
``qparse``. If you only want to see the SOFTSUSY SLHA output, you can use the
``-s`` or ``--slha`` flag. (Note, it will go to stderr, not stdout.) Currently
the SLHA output will not include any of the observables, as in FLHA files. The
help output for the utility is as follows::

  General command:
  qpoint [options] -- <m0> [MH1 MH2] [mq ml] [m3rd] <MG1 MG2 MG3 | mhf> <At Ab Atau | A0> <tb> <sgn(mu)>
  
  The following options are available:
    -H, --help                 print this message
    -s, --slha                 output SLHA to stderr
  
  Non-universality flags:
    -h, --higgs                seperate masses for Higgs fields, requires MH1 and MH2
    -g, --gaugino                 ... for gaugino fields, requires MG1, MG2, and MG3 instead of mhf
    -t, --trilinear               ... for trilinear couplings, requires At, Ab, and Atau instead of A0
    -3, --3rd-gen                 ... for 3rd gen sfermions, requires m3rd
    -f, --mq-ml                   ... for squarks and sleptons, requires mq and ml
    -F, --full-sugra           all masses and couplings are specified
  
  Optional nuisance parameters:
        --alpha-s=VAL          alpha(strong), at scale MZ, by default:0.1184
        --alpha-em-inv=VAL     1/alpha(EM), at scale MZ, by default:127.933
        --mtop=VAL             top quark pole mass, by default:173.07
        --mbmb=VAL             running bottom quark mass mb(mb), by default:4.18

(Masses are specified in GeV.)

This lets you specify a sugra point with a few different types of boundary
conditions. For example, for a plain mSUGRA scenario the format to follow would
be::

  qpoint [options] -- <m0> <mhf> <A0> <tan beta> <sgn(mu)>

For example, we could With the top quark mass set to ``175 GeV``, ``m0 = 200
GeV``, ``mhf = 100 GeV``, ``A0 = 0``, ``tanβ = 15``, ``sgn(μ) = 1``::

  qpoint --mtop=175 -- 200 100 0 15 1

As a more complicated example, we could also adjust the bottom quark mass,
specify different masses for the 3rd generation sfermions, and for the gauginos,
while requesting SLHA output as such::

  qpoint -s3g --mtop=<mt> --mbmb=<mb> -- <m0> <m3rd> <MG1> <MG2> <MG3> <A0> <tb> <sgn(mu)>


