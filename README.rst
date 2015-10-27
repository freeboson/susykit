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


