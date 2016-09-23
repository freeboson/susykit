/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - qpoint_softsusy_opts.hpp:                                                -
    -                                                                          -
    - This file is part of SusyKit, https://freeboson.org/susykit/.            -
    -                                                                          -
    - SusyKit is free software: you can redistribute it and/or modify it under -
    - the terms of the GNU Lesser General Public License as published by the   -
    - Free Software Foundation, either version 3 of the License, or (at your   -
    - option) any later version.                                               -
    -                                                                          -
    - SusyKit is distributed in the hope that it will be useful, but WITHOUT   -
    - ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    -
    - FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     -
    - License for more details.                                                -
    -                                                                          -
    - You should have received a copy of the GNU Lesser General Public License -
    - along with SusyKit.  If not, see <http://www.gnu.org/licenses/>.         -
    -                                                                          -
    ----------------------------------------------------------------------------

*/


#pragma once
#ifndef QPOINT_SOFTSUSY_OPTS_HPP
#define QPOINT_SOFTSUSY_OPTS_HPP

#include "susykit/softsusy_opts.hpp"


class qpoint_opts : public softsusy_opts {
public:
    qpoint_opts(int argc, char **argv);

    void usage() const;

    bool slha() const { return slha_format; }

private:

    int opthandle(int argc, char **argv);

    void make_pars(int argi, char **argv);

    bool gaugino;
    bool higgs;
    bool trilinear;
    bool thirdgen;
    bool mq_ml;
    bool full_sugra;

    bool slha_format;
};

#endif


