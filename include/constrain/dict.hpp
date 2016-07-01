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


#pragma once
#ifndef DICT_HPP
#define DICT_HPP

#include <string>
#include <list>
#include <map>

namespace susy_dict {
    // MODSEL block
    const std::string model_string = "model";

    const std::string mGUT = "mGUT";

    // SMINPUTS block
    const std::string alpha_em_inv = "alpha_em_inv"; // at Mz
    const std::string g_fermi = "g_fermi";
    const std::string alpha_s = "alpha_s"; // at Mz
    const std::string m_z = "m_z"; // Z mass (pole)
    const std::string m_b = "m_b"; // mb(mb)
    const std::string m_top = "m_top"; // top mass (pole)
    const std::string m_tau = "m_tau"; // tau mass (pole)

    // MINPAR block
    const std::string m0 = "m0";    // these are the
    const std::string mhf = "mhf";    // usual mSUGRA inputs
    const std::string tb = "tb";
    const std::string sgnmu = "sgnmu"; //not all of these are used in NUSUGRA
    const std::string a0 = "a0";

    // EXTPAR block
    const std::string m1_X = "m1_X"; // NUSUGRA ONLY
    const std::string m2_X = "m2_X";
    const std::string m3_X = "m3_X";
    const std::string at_X = "at_X";
    const std::string ab_X = "ab_X";
    const std::string atau_X = "atau_X";
    const std::string m_h1sq_X = "m_h1sq_X";
    const std::string m_h2sq_X = "m_h2sq_X";
    const std::string m_e_l_X = "m_e_l_X";
    const std::string m_mu_l_X = "m_mu_l_X";
    const std::string m_tau_l_X = "m_tau_l_X";
    const std::string m_e_r_X = "m_e_r_X";
    const std::string m_mu_r_X = "m_mu_r_X";
    const std::string m_tau_r_X = "m_tau_r_X";
    const std::string m_ql1_X = "m_ql1_X";
    const std::string m_ql2_X = "m_ql2_X";
    const std::string m_ql3_X = "m_ql3_X";
    const std::string m_u_r_X = "m_u_r_X";
    const std::string m_c_r_X = "m_c_r_X";
    const std::string m_t_r_X = "m_t_r_X";
    const std::string m_d_r_X = "m_d_r_X";
    const std::string m_s_r_X = "m_s_r_X";
    const std::string m_b_r_X = "m_b_r_X";


    // MASS block
    const std::string m_w = "m_w";
    const std::string m_h0 = "m_h0";
    const std::string m_H0 = "m_H0";
    const std::string m_A0 = "m_A0";
    const std::string m_Hpm = "m_Hpm";
    const std::string m_g = "m_g";
    const std::string m_o1 = "m_o1";
    const std::string m_o2 = "m_o2";
    const std::string m_1pm = "m_1pm";
    const std::string m_o3 = "m_o3";
    const std::string m_o4 = "m_o4";
    const std::string m_2pm = "m_2pm";
    const std::string m_d_l = "m_d_l";
    const std::string m_u_l = "m_u_l";
    const std::string m_s_l = "m_s_l";
    const std::string m_c_l = "m_c_l";
    const std::string m_b_1 = "m_b_1";
    const std::string m_t_1 = "m_t_1";
    const std::string m_e_l = "m_e_l";
    const std::string m_nue_l = "m_nue_l";
    const std::string m_mu_l = "m_mu_l";
    const std::string m_numu_l = "m_numu_l";
    const std::string m_stau_1 = "m_stau_1";
    const std::string m_nu_tau_l = "m_nu_tau_l";
    const std::string m_d_r = "m_d_r";
    const std::string m_u_r = "m_u_r";
    const std::string m_s_r = "m_s_r";
    const std::string m_c_r = "m_c_r";
    const std::string m_b_2 = "m_b_2";
    const std::string m_t_2 = "m_t_2";
    const std::string m_e_r = "m_e_r";
    const std::string m_mu_r = "m_mu_r";
    const std::string m_stau_2 = "m_stau_2";

    // alpha block
    const std::string higgs_alpha = "higgs_alpha";

    // nmix block
    const std::string nmix_11 = "nmix_11";
    const std::string nmix_12 = "nmix_12";
    const std::string nmix_13 = "nmix_13";
    const std::string nmix_14 = "nmix_14";
    const std::string nmix_21 = "nmix_21";
    const std::string nmix_22 = "nmix_22";
    const std::string nmix_23 = "nmix_23";
    const std::string nmix_24 = "nmix_24";
    const std::string nmix_31 = "nmix_31";
    const std::string nmix_32 = "nmix_32";
    const std::string nmix_33 = "nmix_33";
    const std::string nmix_34 = "nmix_34";
    const std::string nmix_41 = "nmix_41";
    const std::string nmix_42 = "nmix_42";
    const std::string nmix_43 = "nmix_43";
    const std::string nmix_44 = "nmix_44";

    // umix block
    const std::string umix_11 = "umix_11";
    const std::string umix_12 = "umix_12";
    const std::string umix_21 = "umix_21";
    const std::string umix_22 = "umix_22";

    // vmix block
    const std::string vmix_11 = "vmix_11";
    const std::string vmix_12 = "vmix_12";
    const std::string vmix_21 = "vmix_21";
    const std::string vmix_22 = "vmix_22";

    // stopmix block
    const std::string stopmix_11 = "stopmix_11";
    const std::string stopmix_12 = "stopmix_12";
    const std::string stopmix_21 = "stopmix_21";
    const std::string stopmix_22 = "stopmix_22";

    // sbotmix block
    const std::string sbotmix_11 = "sbotmix_11";
    const std::string sbotmix_12 = "sbotmix_12";
    const std::string sbotmix_21 = "sbotmix_21";
    const std::string sbotmix_22 = "sbotmix_22";

    // staumix block
    const std::string staumix_11 = "staumix_11";
    const std::string staumix_12 = "staumix_12";
    const std::string staumix_21 = "staumix_21";
    const std::string staumix_22 = "staumix_22";

    // gauge block
    const std::string gauge_q = "gauge_q";
    const std::string gauge_gp = "gauge_gp";
    const std::string gauge_g = "gauge_g";
    const std::string gauge_g3 = "gauge_g3";

    // yukawa blocks (yu, yd, ye)
    const std::string yu_q = "yukawa_q";
    const std::string yt = "yt";
    const std::string yb = "yb";
    const std::string ytau = "ytau";

    // hmix block
    const std::string hmix_q = "hmix_q";
    const std::string hmix_mu = "hmix_mu";
    const std::string hmix_tb = "hmix_tb";
    const std::string hmix_vev = "hmix_vev";
    const std::string hmix_ma2 = "hmix_ma2";

    // msoft block
    const std::string msoft_q = "msoft_q";
    const std::string m1_q = "m1_q";
    const std::string m2_q = "m2_q";
    const std::string m3_q = "m3_q";
    const std::string m_h1sq_q = "m_h1sq_q";
    const std::string m_h2sq_q = "m_h2sq_q";
    const std::string m_e_l_q = "m_e_l_q";
    const std::string m_mu_l_q = "m_mu_l_q";
    const std::string m_tau_l_q = "m_tau_l_q";
    const std::string m_e_r_q = "m_e_r_q";
    const std::string m_mu_r_q = "m_mu_r_q";
    const std::string m_tau_r_q = "m_tau_r_q";
    const std::string m_ql1_q = "m_ql1_q";
    const std::string m_ql2_q = "m_ql2_q";
    const std::string m_ql3_q = "m_ql3_q";
    const std::string m_u_r_q = "m_u_r_q";
    const std::string m_c_r_q = "m_c_r_q";
    const std::string m_t_r_q = "m_t_r_q";
    const std::string m_d_r_q = "m_d_r_q";
    const std::string m_s_r_q = "m_s_r_q";
    const std::string m_b_r_q = "m_b_r_q";

    // au block
    const std::string q_au = "q_au"; // breaking convention slightly..
    const std::string au_q = "au_q";
    const std::string ac_q = "ac_q";
    const std::string at_q = "at_q";

    // ad block
    const std::string q_ad = "q_ad"; // breaking convention slightly..
    const std::string ad_q = "ad_q";
    const std::string as_q = "as_q";
    const std::string ab_q = "ab_q";

    // ae block
    const std::string q_ae = "q_ae"; // breaking convention slightly..
    const std::string ae_q = "ae_q";
    const std::string amu_q = "amu_q";
    const std::string atau_q = "atau_q";

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

        const std::list<std::string> observe_row = {

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
        };
    }

    // whatever you do, don't mess this up!!
    const std::list<std::string> mSUGRA_row = {

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
    };

    const std::list<std::string> NUSUGRA_row = {

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
    };

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


