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


#include <sstream>
#include <iomanip>
#include "constrain/dict.hpp"
#include "constrain/get_slha.hpp"

using namespace std;
using namespace susy_dict;

#define STRINGIFY(s) STRINGIFY_EX(s)
#define STRINGIFY_EX(s) #s

// this is a useful macro for the lines of a block
// that have matrix elements listed. just pass it 
// the literal root of the matrix variables, and 
// then two literal integers for the row/col
// e.g., matblock(staumix,1,3) writes a line 
// for the variable staumix_13
#define matblock(name, i, j) right << setw(3) << i << setw(3) << j << setw(19) <<  m->get_datum( name ## _ ## i ## j )

const char nl = '\n';

string get_slha::operator()(const model &m) const {
    ostringstream oss;
    oss << sp_modsel(&m)
    << sminputs(&m)
    << minpar(&m)
    << extpar(&m)
    << mass(&m)
    << alpha(&m)
    << nmix(&m)
    << umix(&m)
    << vmix(&m)
    << stopmix(&m)
    << sbotmix(&m)
    << staumix(&m)
    << gauge(&m)
    << yukawa(&m)
    << hmix(&m)
    << msoft(&m)
    << au(&m)
    << ad(&m)
    << ae(&m);
    return oss.str();
}

string get_slha::sp_modsel(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block SPINFO               # Program information" << nl
    << "1   SOFTSUSY               # spectrum calculator" << nl
    << "2   " << STRINGIFY(SOFTSUSY_VERSION) << "                  # version number" << nl
    << "Block MODSEL               # Select model" << nl;
    if (m->get_model_type() == model::mSUGRA) {
        oss << "1    1                     # sugra" << nl;
    }
    else {
        oss << "1    0                     # nonUniversal" << nl;
    }
    return oss.str();
}

string get_slha::sminputs(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block SMINPUTS   # Standard Model inputs" << nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(alpha_em_inv) << "   # alpha_em^(-1)(MZ) SM MSBar" <<
    nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(g_fermi) << "   # G_Fermi" << nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(alpha_s) << "   # alpha_s(MZ) MSBar" << nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_z) << "   # MZ(pole)" << nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_b) << "   # mb(mb)" << nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_top) << "   # Mtop(pole)" << nl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_tau) << "   # Mtau(pole)" << nl;

    return oss.str();
}

string get_slha::minpar(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    if (m->get_model_type() == model::mSUGRA) {
        oss << "Block MINPAR  # SUSY breaking input parameters" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(m0) << "   # m0" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(mhf) << "   # mhf" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(tb) << "   # tan beta" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(sgnmu) << "   # sgn(mu)" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(a0) << "   # A_0" << nl;
    }
    else {
        i = 2; // for nonUniversal, we start at (2+1)
        oss << "Block MINPAR  # SUSY breaking input parameters" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(tb) << "   # tan beta" << nl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(sgnmu) << "   # sgn(mu)" << nl;
    }

    return oss.str();
}

string get_slha::extpar(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    if (m->get_model_type() == model::mSUGRA) // no extpar for mSUGRA
        return string("");

    oss
    << "Block EXTPAR" << "  # non-universal SUSY breaking parameters" << nl
    << right << setw(6) << "1" << setw(19) << m->get_datum(m1_X) << "  # M_1(MX)" << nl
    << right << setw(6) << "2" << setw(19) << m->get_datum(m2_X) << "  # M_2(MX)" << nl
    << right << setw(6) << "3" << setw(19) << m->get_datum(m3_X) << "  # M_3(MX)" << nl
    << right << setw(6) << "11" << setw(19) << m->get_datum(at_X) << "  # At(MX)" << nl
    << right << setw(6) << "12" << setw(19) << m->get_datum(ab_X) << "  # Ab(MX)" << nl
    << right << setw(6) << "13" << setw(19) << m->get_datum(atau_X) << "  # Atau(MX)" << nl
    << right << setw(6) << "21" << setw(19) << m->get_datum(m_h1sq_X) << "  # mHd^2(MX)" << nl
    << right << setw(6) << "22" << setw(19) << m->get_datum(m_h2sq_X) << "  # mHu^2(MX)" << nl
    << right << setw(6) << "31" << setw(19) << m->get_datum(m_e_l_X) << "  # meL(MX)" << nl
    << right << setw(6) << "32" << setw(19) << m->get_datum(m_mu_l_X) << "  # mmuL(MX)" << nl
    << right << setw(6) << "33" << setw(19) << m->get_datum(m_tau_l_X) << "  # mtauL(MX)" << nl
    << right << setw(6) << "34" << setw(19) << m->get_datum(m_e_r_X) << "  # meR(MX)" << nl
    << right << setw(6) << "35" << setw(19) << m->get_datum(m_mu_r_X) << "  # mmuR(MX)" << nl
    << right << setw(6) << "36" << setw(19) << m->get_datum(m_tau_r_X) << "  # mtauR(MX)" << nl
    << right << setw(6) << "41" << setw(19) << m->get_datum(m_ql1_X) << "  # mqL1(MX)" << nl
    << right << setw(6) << "42" << setw(19) << m->get_datum(m_ql2_X) << "  # mqL2(MX)" << nl
    << right << setw(6) << "43" << setw(19) << m->get_datum(m_ql3_X) << "  # mqL3(MX)" << nl
    << right << setw(6) << "44" << setw(19) << m->get_datum(m_u_r_X) << "  # muR(MX)" << nl
    << right << setw(6) << "45" << setw(19) << m->get_datum(m_c_r_X) << "  # mcR(MX)" << nl
    << right << setw(6) << "46" << setw(19) << m->get_datum(m_t_r_X) << "  # mtR(MX)" << nl
    << right << setw(6) << "47" << setw(19) << m->get_datum(m_d_r_X) << "  # mdR(MX)" << nl
    << right << setw(6) << "48" << setw(19) << m->get_datum(m_s_r_X) << "  # msR(MX)" << nl
    << right << setw(6) << "49" << setw(19) << m->get_datum(m_b_r_X) << "  # mbR(MX)" << nl;

    return oss.str();
}

string get_slha::mass(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "# mgut=" << m->get_datum(mGUT) << " GeV" << nl
    << "Block MASS   # Mass spectrum" << nl
    << "# PDG code     mass             particle" << nl
    << right << setw(10) << pdg.find(m_w)->second << setw(19) << m->get_datum(m_w) << "   # MW" << nl
    << right << setw(10) << pdg.find(m_h0)->second << setw(19) << m->get_datum(m_h0) << "   # h0" << nl
    << right << setw(10) << pdg.find(m_H0)->second << setw(19) << m->get_datum(m_H0) << "   # H0" << nl
    << right << setw(10) << pdg.find(m_A0)->second << setw(19) << m->get_datum(m_A0) << "   # A0" << nl
    << right << setw(10) << pdg.find(m_Hpm)->second << setw(19) << m->get_datum(m_Hpm) << "   # H+" << nl
    << right << setw(10) << pdg.find(m_g)->second << setw(19) << m->get_datum(m_g) << "   # ~g" << nl
    << right << setw(10) << pdg.find(m_o1)->second << setw(19) << m->get_datum(m_o1) << "   # ~neutralino(1)" << nl
    << right << setw(10) << pdg.find(m_o2)->second << setw(19) << m->get_datum(m_o2) << "   # ~neutralino(2)" << nl
    << right << setw(10) << pdg.find(m_1pm)->second << setw(19) << m->get_datum(m_1pm) << "   # ~chargino(1)" << nl
    << right << setw(10) << pdg.find(m_o3)->second << setw(19) << m->get_datum(m_o3) << "   # ~neutralino(3)" << nl
    << right << setw(10) << pdg.find(m_o4)->second << setw(19) << m->get_datum(m_o4) << "   # ~neutralino(4)" << nl
    << right << setw(10) << pdg.find(m_2pm)->second << setw(19) << m->get_datum(m_2pm) << "   # ~chargino(2)" << nl
    << right << setw(10) << pdg.find(m_d_l)->second << setw(19) << m->get_datum(m_d_l) << "   # ~d_L" << nl
    << right << setw(10) << pdg.find(m_u_l)->second << setw(19) << m->get_datum(m_u_l) << "   # ~u_L" << nl
    << right << setw(10) << pdg.find(m_s_l)->second << setw(19) << m->get_datum(m_s_l) << "   # ~s_L" << nl
    << right << setw(10) << pdg.find(m_c_l)->second << setw(19) << m->get_datum(m_c_l) << "   # ~c_L" << nl
    << right << setw(10) << pdg.find(m_b_1)->second << setw(19) << m->get_datum(m_b_1) << "   # ~b_1" << nl
    << right << setw(10) << pdg.find(m_t_1)->second << setw(19) << m->get_datum(m_t_1) << "   # ~t_1" << nl
    << right << setw(10) << pdg.find(m_e_l)->second << setw(19) << m->get_datum(m_e_l) << "   # ~e_L" << nl
    << right << setw(10) << pdg.find(m_nue_l)->second << setw(19) << m->get_datum(m_nue_l) << "   # ~nue_L" << nl
    << right << setw(10) << pdg.find(m_mu_l)->second << setw(19) << m->get_datum(m_mu_l) << "   # mu_L" << nl
    << right << setw(10) << pdg.find(m_numu_l)->second << setw(19) << m->get_datum(m_numu_l) << "   # ~numu_L" << nl
    << right << setw(10) << pdg.find(m_stau_1)->second << setw(19) << m->get_datum(m_stau_1) << "   # ~stau_1" << nl
    << right << setw(10) << pdg.find(m_nu_tau_l)->second << setw(19) << m->get_datum(m_nu_tau_l) << "   # ~nu_tau_L" <<
    nl
    << right << setw(10) << pdg.find(m_d_r)->second << setw(19) << m->get_datum(m_d_r) << "   # ~d_R" << nl
    << right << setw(10) << pdg.find(m_u_r)->second << setw(19) << m->get_datum(m_u_r) << "   # ~u_R" << nl
    << right << setw(10) << pdg.find(m_s_r)->second << setw(19) << m->get_datum(m_s_r) << "   # ~s_R" << nl
    << right << setw(10) << pdg.find(m_c_r)->second << setw(19) << m->get_datum(m_c_r) << "   # ~c_R" << nl
    << right << setw(10) << pdg.find(m_b_2)->second << setw(19) << m->get_datum(m_b_2) << "   # ~b_2" << nl
    << right << setw(10) << pdg.find(m_t_2)->second << setw(19) << m->get_datum(m_t_2) << "   # ~t_2" << nl
    << right << setw(10) << pdg.find(m_e_r)->second << setw(19) << m->get_datum(m_e_r) << "   # ~e_R" << nl
    << right << setw(10) << pdg.find(m_mu_r)->second << setw(19) << m->get_datum(m_mu_r) << "   # ~mu_R" << nl
    << right << setw(10) << pdg.find(m_stau_2)->second << setw(19) << m->get_datum(m_stau_2) << "   # ~stau_2" << nl;

    return oss.str();
}

string get_slha::alpha(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "# Higgs mixing" << nl
    << "Block alpha   # Effective Higgs mixing parameter" << nl
    << right << setw(25) << m->get_datum(higgs_alpha) << "        # alpha" << nl;

    return oss.str();
}


string get_slha::nmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block nmix  # neutralino mixing matrix" << nl
    << matblock(nmix, 1, 1) << nl
    << matblock(nmix, 1, 2) << nl
    << matblock(nmix, 1, 3) << nl
    << matblock(nmix, 1, 4) << nl
    << matblock(nmix, 2, 1) << nl
    << matblock(nmix, 2, 2) << nl
    << matblock(nmix, 2, 3) << nl
    << matblock(nmix, 2, 4) << nl
    << matblock(nmix, 3, 1) << nl
    << matblock(nmix, 3, 2) << nl
    << matblock(nmix, 3, 3) << nl
    << matblock(nmix, 3, 4) << nl
    << matblock(nmix, 4, 1) << nl
    << matblock(nmix, 4, 2) << nl
    << matblock(nmix, 4, 3) << nl
    << matblock(nmix, 4, 4) << nl;

    return oss.str();
}

string get_slha::umix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block Umix  # Chargino U mixing matrix" << nl
    << matblock(umix, 1, 1) << nl
    << matblock(umix, 1, 2) << nl
    << matblock(umix, 2, 1) << nl
    << matblock(umix, 2, 2) << nl;

    return oss.str();
}

string get_slha::vmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block Vmix  # Chargino V mixing matrix" << nl
    << matblock(vmix, 1, 1) << nl
    << matblock(vmix, 1, 2) << nl
    << matblock(vmix, 2, 1) << nl
    << matblock(vmix, 2, 2) << nl;

    return oss.str();
}

string get_slha::stopmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block stopmix  # stop mixing matrix" << nl
    << matblock(stopmix, 1, 1) << nl
    << matblock(stopmix, 1, 2) << nl
    << matblock(stopmix, 2, 1) << nl
    << matblock(stopmix, 2, 2) << nl;

    return oss.str();
}

string get_slha::sbotmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block sbotmix  # sbottom mixing matrix" << nl
    << matblock(sbotmix, 1, 1) << nl
    << matblock(sbotmix, 1, 2) << nl
    << matblock(sbotmix, 2, 1) << nl
    << matblock(sbotmix, 2, 2) << nl;

    return oss.str();
}

string get_slha::staumix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block staumix  # stau mixing matrix" << nl
    << matblock(staumix, 1, 1) << nl
    << matblock(staumix, 1, 2) << nl
    << matblock(staumix, 2, 1) << nl
    << matblock(staumix, 2, 2) << nl;

    return oss.str();
}


string get_slha::gauge(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block gauge Q= " << m->get_datum(gauge_q) << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(gauge_gp) << "   # g'(Q) MSSM DRbar" << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(gauge_g) << "   # g(Q) MSSM DRbar" << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(gauge_g3) << "   # g3(Q) MSSM DRbar" << nl;

    return oss.str();
}

string get_slha::yukawa(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block yu Q= " << m->get_datum(yu_q) << nl
    << "  3  3" << right << setw(19) << m->get_datum(yt) << "   # Yt(Q) MSSM DRbar" << nl
    << "Block yd Q= " << m->get_datum(yu_q) << nl
    << "  3  3" << right << setw(19) << m->get_datum(yb) << "   # Yb(Q) MSSM DRbar" << nl
    << "Block ye Q= " << m->get_datum(yu_q) << nl
    << "  3  3" << right << setw(19) << m->get_datum(ytau) << "   # Ytau(Q) MSSM DRbar" << nl;

    return oss.str();
}

string get_slha::hmix(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block hmix Q= " << m->get_datum(hmix_q) << "    # Higgs mixing parameters" << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_mu) << "    # mu(Q) MSSM DRbar" << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_tb) << "    # tanbeta(Q) MSSM DRbar" << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_vev) << "    # Higgs VEV(Q) MSSM DRbar" << nl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_ma2) << "    # mA^2(Q) MSSM DRbar" << nl;

    return oss.str();
}

string get_slha::msoft(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block msoft Q= " << m->get_datum(msoft_q) << "      # MSSM DRbar SUSY breaking parameters" << nl
    << right << setw(6) << "1" << setw(19) << m->get_datum(m1_q) << "      # M_1(Q)" << nl
    << right << setw(6) << "2" << setw(19) << m->get_datum(m2_q) << "      # M_2(Q)" << nl
    << right << setw(6) << "3" << setw(19) << m->get_datum(m3_q) << "      # M_3(Q)" << nl
    << right << setw(6) << "21" << setw(19) << m->get_datum(m_h1sq_q) << "      # MH1^2(Q)" << nl
    << right << setw(6) << "22" << setw(19) << m->get_datum(m_h2sq_q) << "      # MH2^2(Q)" << nl
    << right << setw(6) << "31" << setw(19) << m->get_datum(m_e_l_q) << "      # meL(Q)" << nl
    << right << setw(6) << "32" << setw(19) << m->get_datum(m_mu_l_q) << "      # mmuL(Q)" << nl
    << right << setw(6) << "33" << setw(19) << m->get_datum(m_tau_l_q) << "      # mtauL(Q)" << nl
    << right << setw(6) << "34" << setw(19) << m->get_datum(m_e_r_q) << "      # meR(Q)" << nl
    << right << setw(6) << "35" << setw(19) << m->get_datum(m_mu_r_q) << "      # mmuR(Q)" << nl
    << right << setw(6) << "36" << setw(19) << m->get_datum(m_tau_r_q) << "      # mtauR(Q)" << nl
    << right << setw(6) << "41" << setw(19) << m->get_datum(m_ql1_q) << "      # mqL1(Q)" << nl
    << right << setw(6) << "42" << setw(19) << m->get_datum(m_ql2_q) << "      # mqL2(Q)" << nl
    << right << setw(6) << "43" << setw(19) << m->get_datum(m_ql3_q) << "      # mqL3(Q)" << nl
    << right << setw(6) << "44" << setw(19) << m->get_datum(m_u_r_q) << "      # muR(Q)" << nl
    << right << setw(6) << "45" << setw(19) << m->get_datum(m_c_r_q) << "      # mcR(Q)" << nl
    << right << setw(6) << "46" << setw(19) << m->get_datum(m_t_r_q) << "      # mtR(Q)" << nl
    << right << setw(6) << "47" << setw(19) << m->get_datum(m_d_r_q) << "      # mdR(Q)" << nl
    << right << setw(6) << "48" << setw(19) << m->get_datum(m_s_r_q) << "      # msR(Q)" << nl
    << right << setw(6) << "49" << setw(19) << m->get_datum(m_b_r_q) << "      # mbR(Q)" << nl;

    return oss.str();
}

string get_slha::au(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block au Q= " << m->get_datum(q_au) << nl; i++;
    oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(au_q) << "      # Au(Q) MSSM DRbar" <<
    nl; i++;
    oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(ac_q) << "      # Ac(Q) MSSM DRbar" <<
    nl; i++;
    oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(at_q) << "      # At(Q) MSSM DRbar" <<
    nl;

    return oss.str();
}

string get_slha::ad(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block ad Q= " << m->get_datum(q_ad) << nl;
    i++; oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(ad_q) << "      # Ad(Q) MSSM DRbar" <<
    nl;
    i++; oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(as_q) << "      # As(Q) MSSM DRbar" <<
    nl;
    i++; oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(ab_q) << "      # Ab(Q) MSSM DRbar" <<
    nl;

    return oss.str();
}

string get_slha::ae(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block ae Q= " << m->get_datum(q_ae) << nl;
    i++; oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(ae_q) << "      # Ae(Q) MSSM DRbar" <<
    nl;
    i++; oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(amu_q) << "      # Amu(Q) MSSM DRbar" <<
    nl;
    i++; oss << right << setw(3) << i << setw(3) << i << setw(19) << m->get_datum(atau_q) <<
    "      # Atau(Q) MSSM DRbar" << nl;

    return oss.str();
}


