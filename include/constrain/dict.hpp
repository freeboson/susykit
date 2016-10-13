/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - dict.hpp:                                                                -
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
#ifndef DICT_HPP
#define DICT_HPP

#include <string>
#if defined(SUSYKIT_IS_IT_1Z_YET)
#  include <experimental/array>
#else
#  include <list>
#endif
#include <unordered_map>

namespace susy_dict {

    enum class slha {

        // MODSEL block
        model_string,

        mGUT,

        // SMINPUTS block
        alpha_em_inv, // at Mz
        g_fermi,
        alpha_s, // at Mz
        m_z, // Z mass (pole)
        m_b, // mb(mb)
        m_top, // top mass (pole)
        m_tau, // tau mass (pole)

        // MINPAR block
        // these are the usual mSUGRA inputs
        // some are in NUSUGRA as well
        m0, mhf, tb, sgnmu, a0,

        // EXTPAR block
        // these are only for NUSUGRA
        m1_X, m2_X, m3_X, // EXTPAR is new
        at_X, ab_X, atau_X,
        m_h1sq_X, m_h2sq_X,
        m_e_l_X, m_mu_l_X, m_tau_l_X,
        m_e_r_X, m_mu_r_X, m_tau_r_X,
        m_ql1_X, m_ql2_X, m_ql3_X,
        m_u_r_X, m_c_r_X, m_t_r_X,
        m_d_r_X, m_s_r_X, m_b_r_X,

        // MASS block
        m_w,
        m_h0, m_H0, m_A0, m_Hpm,
        m_g,
        m_o1, m_o2, m_1pm, m_o3, m_o4, m_2pm,
        m_d_l, m_u_l, m_s_l,
        m_c_l, m_b_1, m_t_1,
        m_e_l, m_nue_l,
        m_mu_l, m_numu_l,
        m_stau_1, m_nu_tau_l,
        m_d_r, m_u_r, m_s_r,
        m_c_r, m_b_2, m_t_2,
        m_e_r, m_mu_r, m_stau_2,

        // alpha block
        higgs_alpha,

        // nmix block
        nmix_11, nmix_21, nmix_31, nmix_41,
        nmix_12, nmix_22, nmix_32, nmix_42,
        nmix_13, nmix_23, nmix_33, nmix_43,
        nmix_14, nmix_24, nmix_34, nmix_44,

        // umix block
        umix_11, umix_12,
        umix_21, umix_22,

        // vmix block
        vmix_11, vmix_12,
        vmix_21, vmix_22,

        // stopmix block
        stopmix_11, stopmix_12,
        stopmix_21, stopmix_22,

        // sbotmix block
        sbotmix_11, sbotmix_12,
        sbotmix_21, sbotmix_22,

        // staumix block
        staumix_11, staumix_12,
        staumix_21, staumix_22,

        // gauge block
        q_gauge, gauge_gp, gauge_g, gauge_g3,

        // yukawa blocks (yu, yd, ye)
        q_yukawa, yt, yb, ytau,

        // hmix block
        q_hmix, hmix_mu, hmix_tb, hmix_vev, hmix_ma2,

        // msoft block
        q_msoft,
        m1_q, m2_q, m3_q,
        m_h1sq_q, m_h2sq_q,
        m_e_l_q, m_mu_l_q, m_tau_l_q,
        m_e_r_q, m_mu_r_q, m_tau_r_q,
        m_ql1_q, m_ql2_q, m_ql3_q,
        m_u_r_q, m_c_r_q, m_t_r_q,
        m_d_r_q, m_s_r_q, m_b_r_q,

        // au block
        q_au, au_q, ac_q, at_q,

        // ad block
        q_ad, ad_q, as_q, ab_q,

        // ae block
        q_ae, ae_q, amu_q, atau_q
    };

    const std::unordered_map<std::string, slha> slha_str_key = {
            // MODSEL block
            {"model",        model_string},

            {"mGUT",         mGUT},

            // SMINPUTS block
            {"alpha_em_inv", alpha_em_inv}, // at Mz
            {"g_fermi",      g_fermi},
            {"alpha_s",      alpha_s}, // at Mz
            {"m_z",          m_z}, // Z mass (pole)
            {"m_b",          m_b}, // mb(mb)
            {"m_top",        m_top}, // top mass (pole)
            {"m_tau",        m_tau}, // tau mass (pole)

            // MINPAR block
            {"m0",           m0},    // these are the
            {"mhf",          mhf},    // usual mSUGRA inputs
            {"tb",           tb},
            {"sgnmu",        sgnmu}, //not all of these are used in NUSUGRA
            {"a0",           a0},

            // EXTPAR block
            {"m1_X",         m1_X}, // NUSUGRA ONLY
            {"m2_X",         m2_X},
            {"m3_X",         m3_X},
            {"at_X",         at_X},
            {"ab_X",         ab_X},
            {"atau_X",       atau_X},
            {"m_h1sq_X",     m_h1sq_X},
            {"m_h2sq_X",     m_h2sq_X},
            {"m_e_l_X",      m_e_l_X},
            {"m_mu_l_X",     m_mu_l_X},
            {"m_tau_l_X",    m_tau_l_X},
            {"m_e_r_X",      m_e_r_X},
            {"m_mu_r_X",     m_mu_r_X},
            {"m_tau_r_X",    m_tau_r_X},
            {"m_ql1_X",      m_ql1_X},
            {"m_ql2_X",      m_ql2_X},
            {"m_ql3_X",      m_ql3_X},
            {"m_u_r_X",      m_u_r_X},
            {"m_c_r_X",      m_c_r_X},
            {"m_t_r_X",      m_t_r_X},
            {"m_d_r_X",      m_d_r_X},
            {"m_s_r_X",      m_s_r_X},
            {"m_b_r_X",      m_b_r_X},


            // MASS block
            {"m_w",          m_w},
            {"m_h0",         m_h0},
            {"m_H0",         m_H0},
            {"m_A0",         m_A0},
            {"m_Hpm",        m_Hpm},
            {"m_g",          m_g},
            {"m_o1",         m_o1},
            {"m_o2",         m_o2},
            {"m_1pm",        m_1pm},
            {"m_o3",         m_o3},
            {"m_o4",         m_o4},
            {"m_2pm",        m_2pm},
            {"m_d_l",        m_d_l},
            {"m_u_l",        m_u_l},
            {"m_s_l",        m_s_l},
            {"m_c_l",        m_c_l},
            {"m_b_1",        m_b_1},
            {"m_t_1",        m_t_1},
            {"m_e_l",        m_e_l},
            {"m_nue_l",      m_nue_l},
            {"m_mu_l",       m_mu_l},
            {"m_numu_l",     m_numu_l},
            {"m_stau_1",     m_stau_1},
            {"m_nu_tau_l",   m_nu_tau_l},
            {"m_d_r",        m_d_r},
            {"m_u_r",        m_u_r},
            {"m_s_r",        m_s_r},
            {"m_c_r",        m_c_r},
            {"m_b_2",        m_b_2},
            {"m_t_2",        m_t_2},
            {"m_e_r",        m_e_r},
            {"m_mu_r",       m_mu_r},
            {"m_stau_2",     m_stau_2},

            // alpha block
            {"higgs_alpha",  higgs_alpha},

            // nmix block
            {"nmix_11",      nmix_11},
            {"nmix_12",      nmix_12},
            {"nmix_13",      nmix_13},
            {"nmix_14",      nmix_14},
            {"nmix_21",      nmix_21},
            {"nmix_22",      nmix_22},
            {"nmix_23",      nmix_23},
            {"nmix_24",      nmix_24},
            {"nmix_31",      nmix_31},
            {"nmix_32",      nmix_32},
            {"nmix_33",      nmix_33},
            {"nmix_34",      nmix_34},
            {"nmix_41",      nmix_41},
            {"nmix_42",      nmix_42},
            {"nmix_43",      nmix_43},
            {"nmix_44",      nmix_44},

            // umix block
            {"umix_11",      umix_11},
            {"umix_12",      umix_12},
            {"umix_21",      umix_21},
            {"umix_22",      umix_22},

            // vmix block
            {"vmix_11",      vmix_11},
            {"vmix_12",      vmix_12},
            {"vmix_21",      vmix_21},
            {"vmix_22",      vmix_22},

            // stopmix block
            {"stopmix_11",   stopmix_11},
            {"stopmix_12",   stopmix_12},
            {"stopmix_21",   stopmix_21},
            {"stopmix_22",   stopmix_22},

            // sbotmix block
            {"sbotmix_11",   sbotmix_11},
            {"sbotmix_12",   sbotmix_12},
            {"sbotmix_21",   sbotmix_21},
            {"sbotmix_22",   sbotmix_22},

            // staumix block
            {"staumix_11",   staumix_11},
            {"staumix_12",   staumix_12},
            {"staumix_21",   staumix_21},
            {"staumix_22",   staumix_22},

            // gauge block
            {"a_gauge",      q_gauge},
            {"gauge_gp",     gauge_gp},
            {"gauge_g",      gauge_g},
            {"gauge_g3",     gauge_g3},

            // yukawa blocks (yu, yd, ye)
            {"q_yukawa",     q_yukawa},
            {"yt",           yt},
            {"yb",           yb},
            {"ytau",         ytau},

            // hmix block
            {"q_hmix",       q_hmix},
            {"hmix_mu",      hmix_mu},
            {"hmix_tb",      hmix_tb},
            {"hmix_vev",     hmix_vev},
            {"hmix_ma2",     hmix_ma2},

            // msoft block
            {"q_msoft",      q_msoft},
            {"m1_q",         m1_q},
            {"m2_q",         m2_q},
            {"m3_q",         m3_q},
            {"m_h1sq_q",     m_h1sq_q},
            {"m_h2sq_q",     m_h2sq_q},
            {"m_e_l_q",      m_e_l_q},
            {"m_mu_l_q",     m_mu_l_q},
            {"m_tau_l_q",    m_tau_l_q},
            {"m_e_r_q",      m_e_r_q},
            {"m_mu_r_q",     m_mu_r_q},
            {"m_tau_r_q",    m_tau_r_q},
            {"m_ql1_q",      m_ql1_q},
            {"m_ql2_q",      m_ql2_q},
            {"m_ql3_q",      m_ql3_q},
            {"m_u_r_q",      m_u_r_q},
            {"m_c_r_q",      m_c_r_q},
            {"m_t_r_q",      m_t_r_q},
            {"m_d_r_q",      m_d_r_q},
            {"m_s_r_q",      m_s_r_q},
            {"m_b_r_q",      m_b_r_q},

            // au block
            {"q_au",         q_au},
            {"au_q",         au_q},
            {"ac_q",         ac_q},
            {"at_q",         at_q},

            // ad block
            {"q_ad",         q_ad},
            {"ad_q",         ad_q},
            {"as_q",         as_q},
            {"ab_q",         ab_q},

            // ae block
            {"q_ae",         q_ae},
            {"ae_q",         ae_q},
            {"amu_q",        amu_q},
            {"atau_q",       atau_q}
    };

    namespace observable {
        const std::string micro_valid_bit = "micro_valid_bit";

        const std::string delta_rho = "delta_rho";
        const std::string gmuon = "gmuon";
        const std::string bsgnlo = "bsgnlo";
        const std::string bsmumu = "bsmumu";
        const std::string btaunu = "btaunu";
        const std::string omega = "omega";
        const std::string proton_SI = "proton_SI";
        const std::string proton_SD = "proton_SD";
        const std::string neutron_SI = "neutron_SI";
        const std::string neutron_SD = "neutron_SD";

        const std::string fh_valid_bit = "fh_valid_bit";

        const std::string mw_mssm = "mw_mssm";
        const std::string sw2_mssm = "sw2_mssm";
        const std::string edmeTh = "edmeTh";
        const std::string edmn = "edmn";
        const std::string edmHg = "edmHg";
        const std::string deltaMs_mssm = "deltaMs_mssm";
        const std::string width_h0 = "width_h0";
        const std::string width_H0 = "width_H0";
        const std::string width_A0 = "width_A0";
        const std::string width_Hpm = "width_Hpm";

#if defined(SUSYKIT_IS_IT_1Z_YET)
        const decltype(auto) observe_row = std::experimental::make_array (
#else
        const std::list<std::string> observe_row = {
#endif

            // micromegas OR DarkSUSY/SuperISO
            // TODO: Clean this all up.

            micro_valid_bit,

            delta_rho,
            gmuon,
            bsgnlo,
            bsmumu,
            btaunu,
            omega,
            proton_SI,
            proton_SD,
            neutron_SI,
            neutron_SD,

            // FeynHiggs

            fh_valid_bit,

            mw_mssm,
            sw2_mssm,
            edmeTh,
            edmn,
            edmHg,
            deltaMs_mssm,
            width_h0,
            width_H0,
            width_A0,
            width_Hpm

            // superiso_relic
            //m
#if !defined(SUSYKIT_IS_IT_1Z_YET)
        };
#else
        );
#endif
    }

    // whatever you do, don't mess this up!!
#if defined(SUSYKIT_IS_IT_1Z_YET)
        const decltype(auto) mSUGRA_row = std::experimental::make_array (
#else
        const std::list<std::string> mSUGRA_row = {
#endif

        model_string,

        mGUT,

        alpha_em_inv, g_fermi, alpha_s,
        m_z, m_b, m_top, m_tau,

        m0, mhf, tb, sgnmu, a0,

        m_w,
        m_h0, m_H0, m_A0, m_Hpm,
        m_g,
        m_o1, m_o2, m_1pm, m_o3, m_o4, m_2pm,
        m_d_l, m_u_l, m_s_l,
        m_c_l, m_b_1, m_t_1,
        m_e_l, m_nue_l,
        m_mu_l, m_numu_l,
        m_stau_1, m_nu_tau_l,
        m_d_r, m_u_r, m_s_r,
        m_c_r, m_b_2, m_t_2,
        m_e_r, m_mu_r, m_stau_2,

        higgs_alpha,

#if 0        // nmix is transposed in SLHA files!!
        nmix_11, nmix_12, nmix_13, nmix_14,
        nmix_21, nmix_22, nmix_23, nmix_24,
        nmix_31, nmix_32, nmix_33, nmix_34,
        nmix_41, nmix_42, nmix_43, nmix_44,
#else
        nmix_11, nmix_21, nmix_31, nmix_41,
        nmix_12, nmix_22, nmix_32, nmix_42,
        nmix_13, nmix_23, nmix_33, nmix_43,
        nmix_14, nmix_24, nmix_34, nmix_44,
#endif
        umix_11, umix_12,
        umix_21, umix_22,

        vmix_11, vmix_12,
        vmix_21, vmix_22,

        stopmix_11, stopmix_12,
        stopmix_21, stopmix_22,

        sbotmix_11, sbotmix_12,
        sbotmix_21, sbotmix_22,

        staumix_11, staumix_12,
        staumix_21, staumix_22,

        gauge_q, gauge_gp, gauge_g, gauge_g3,

        yu_q, yt, yb, ytau,

        hmix_q, hmix_mu, hmix_tb, hmix_vev, hmix_ma2,

        msoft_q,
        m1_q, m2_q, m3_q,
        m_h1sq_q, m_h2sq_q,
        m_e_l_q, m_mu_l_q, m_tau_l_q,
        m_e_r_q, m_mu_r_q, m_tau_r_q,
        m_ql1_q, m_ql2_q, m_ql3_q,
        m_u_r_q, m_c_r_q, m_t_r_q,
        m_d_r_q, m_s_r_q, m_b_r_q,

        q_au, au_q, ac_q, at_q,

        q_ad, ad_q, as_q, ab_q,

        q_ae, ae_q, amu_q, atau_q

#if !defined(SUSYKIT_IS_IT_1Z_YET)
    };
#else
    );
#endif

#if defined(SUSYKIT_IS_IT_1Z_YET)
        const decltype(auto) NUSUGRA_row = std::experimental::make_array (
#else
        const std::list<std::string> NUSUGRA_row = {
#endif

        model_string,

        mGUT,

        alpha_em_inv, g_fermi, alpha_s,
        m_z, m_b, m_top, m_tau,

        tb, sgnmu, // MINPAR is different

        m1_X, m2_X, m3_X, // EXTPAR is new
        at_X, ab_X, atau_X,
        m_h1sq_X, m_h2sq_X,
        m_e_l_X, m_mu_l_X, m_tau_l_X,
        m_e_r_X, m_mu_r_X, m_tau_r_X,
        m_ql1_X, m_ql2_X, m_ql3_X,
        m_u_r_X, m_c_r_X, m_t_r_X,
        m_d_r_X, m_s_r_X, m_b_r_X,

        // everything else should be the same
        // i.e. this is DUPLICATE CODE :(
        // TODO: write a function that initializes both
        // 	 mSUGRA_row and NUSUGRA_row to cut out
        // 	 the code duplication

        m_w,
        m_h0, m_H0, m_A0, m_Hpm,
        m_g,
        m_o1, m_o2, m_1pm, m_o3, m_o4, m_2pm,
        m_d_l, m_u_l, m_s_l,
        m_c_l, m_b_1, m_t_1,
        m_e_l, m_nue_l,
        m_mu_l, m_numu_l,
        m_stau_1, m_nu_tau_l,
        m_d_r, m_u_r, m_s_r,
        m_c_r, m_b_2, m_t_2,
        m_e_r, m_mu_r, m_stau_2,

        higgs_alpha,

#if 0        // nmix is transposed in SLHA files!!
        nmix_11, nmix_12, nmix_13, nmix_14,
        nmix_21, nmix_22, nmix_23, nmix_24,
        nmix_31, nmix_32, nmix_33, nmix_34,
        nmix_41, nmix_42, nmix_43, nmix_44,
#else
        nmix_11, nmix_21, nmix_31, nmix_41,
        nmix_12, nmix_22, nmix_32, nmix_42,
        nmix_13, nmix_23, nmix_33, nmix_43,
        nmix_14, nmix_24, nmix_34, nmix_44,
#endif
        umix_11, umix_12,
        umix_21, umix_22,

        vmix_11, vmix_12,
        vmix_21, vmix_22,

        stopmix_11, stopmix_12,
        stopmix_21, stopmix_22,

        sbotmix_11, sbotmix_12,
        sbotmix_21, sbotmix_22,

        staumix_11, staumix_12,
        staumix_21, staumix_22,

        gauge_q, gauge_gp, gauge_g, gauge_g3,

        yu_q, yt, yb, ytau,

        hmix_q, hmix_mu, hmix_tb, hmix_vev, hmix_ma2,

        msoft_q,
        m1_q, m2_q, m3_q,
        m_h1sq_q, m_h2sq_q,
        m_e_l_q, m_mu_l_q, m_tau_l_q,
        m_e_r_q, m_mu_r_q, m_tau_r_q,
        m_ql1_q, m_ql2_q, m_ql3_q,
        m_u_r_q, m_c_r_q, m_t_r_q,
        m_d_r_q, m_s_r_q, m_b_r_q,

        q_au, au_q, ac_q, at_q,

        q_ad, ad_q, as_q, ab_q,

        q_ae, ae_q, amu_q, atau_q

#if !defined(SUSYKIT_IS_IT_1Z_YET)
    };
#else
    );
#endif

    const std::map<std::string, unsigned int> pdg = {
        {m_w,        24},
        {m_h0,       25},
        {m_H0,       35},
        {m_A0,       36},
        {m_Hpm,      37},
        {m_g,        1000021},
        {m_o1,       1000022},
        {m_o2,       1000023},
        {m_1pm,      1000024},
        {m_o3,       1000025},
        {m_o4,       1000035},
        {m_2pm,      1000037},
        {m_d_l,      1000001},
        {m_u_l,      1000002},
        {m_s_l,      1000003},
        {m_c_l,      1000004},
        {m_b_1,      1000005},
        {m_t_1,      1000006},
        {m_e_l,      1000011},
        {m_nue_l,    1000012},
        {m_mu_l,     1000013},
        {m_numu_l,   1000014},
        {m_stau_1,   1000015},
        {m_nu_tau_l, 1000016},
        {m_d_r,      2000001},
        {m_u_r,      2000002},
        {m_s_r,      2000003},
        {m_c_r,      2000004},
        {m_b_2,      2000005},
        {m_t_2,      2000006},
        {m_e_r,      2000011},
        {m_mu_r,     2000013},
        {m_stau_2,   2000015}
    };
}

#endif


