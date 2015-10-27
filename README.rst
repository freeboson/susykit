===========
``SusyKit``
===========

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


