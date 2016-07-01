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

// this is a useful macro for the lines of a block
// that have matrix elements listed. just pass it 
// the literal root of the matrix variables, and 
// then two literal integers for the row/col
// e.g., matblock(staumix,1,3) writes a line 
// for the variable staumix_13
#define matblock(name, i, j) right << setw(3) << i << setw(3) << j << setw(19) <<  m->get_datum( name ## _ ## i ## j )

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
    << "Block SPINFO         # Program information" << endl
    << "1   SOFTSUSY    # spectrum calculator" << endl
    << "2   3.4.0         # version number" << endl
    << "Block MODSEL  # Select model" << endl;
    if (m->get_model_type() == model::mSUGRA) {
        oss << "1    1   # sugra" << endl;
    }
    else {
        oss << "1    0   # nonUniversal" << endl;
    }
    return oss.str();
}

string get_slha::sminputs(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block SMINPUTS   # Standard Model inputs" << endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(alpha_em_inv) << "   # alpha_em^(-1)(MZ) SM MSBar" <<
    endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(g_fermi) << "   # G_Fermi" << endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(alpha_s) << "   # alpha_s(MZ) MSBar" << endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_z) << "   # MZ(pole)" << endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_b) << "   # mb(mb)" << endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_top) << "   # Mtop(pole)" << endl;
    oss << right << setw(6) << ++i << setw(18) << m->get_datum(m_tau) << "   # Mtau(pole)" << endl;

    return oss.str();
}

string get_slha::minpar(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    if (m->get_model_type() == model::mSUGRA) {
        oss << "Block MINPAR  # SUSY breaking input parameters" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(m0) << "   # m0" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(mhf) << "   # mhf" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(tb) << "   # tan beta" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(sgnmu) << "   # sgn(mu)" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(a0) << "   # A_0" << endl;
    }
    else {
        i = 2; // for nonUniversal, we start at (2+1)
        oss << "Block MINPAR  # SUSY breaking input parameters" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(tb) << "   # tan beta" << endl;
        oss << right << setw(6) << ++i << setw(18) << m->get_datum(sgnmu) << "   # sgn(mu)" << endl;
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
    << "Block EXTPAR" << "  # non-universal SUSY breaking parameters" << endl
    << right << setw(6) << "1" << setw(19) << m->get_datum(m1_X) << "  # M_1(MX)" << endl
    << right << setw(6) << "2" << setw(19) << m->get_datum(m2_X) << "  # M_2(MX)" << endl
    << right << setw(6) << "3" << setw(19) << m->get_datum(m3_X) << "  # M_3(MX)" << endl
    << right << setw(6) << "11" << setw(19) << m->get_datum(at_X) << "  # At(MX)" << endl
    << right << setw(6) << "12" << setw(19) << m->get_datum(ab_X) << "  # Ab(MX)" << endl
    << right << setw(6) << "13" << setw(19) << m->get_datum(atau_X) << "  # Atau(MX)" << endl
    << right << setw(6) << "21" << setw(19) << m->get_datum(m_h1sq_X) << "  # mHd^2(MX)" << endl
    << right << setw(6) << "22" << setw(19) << m->get_datum(m_h2sq_X) << "  # mHu^2(MX)" << endl
    << right << setw(6) << "31" << setw(19) << m->get_datum(m_e_l_X) << "  # meL(MX)" << endl
    << right << setw(6) << "32" << setw(19) << m->get_datum(m_mu_l_X) << "  # mmuL(MX)" << endl
    << right << setw(6) << "33" << setw(19) << m->get_datum(m_tau_l_X) << "  # mtauL(MX)" << endl
    << right << setw(6) << "34" << setw(19) << m->get_datum(m_e_r_X) << "  # meR(MX)" << endl
    << right << setw(6) << "35" << setw(19) << m->get_datum(m_mu_r_X) << "  # mmuR(MX)" << endl
    << right << setw(6) << "36" << setw(19) << m->get_datum(m_tau_r_X) << "  # mtauR(MX)" << endl
    << right << setw(6) << "41" << setw(19) << m->get_datum(m_ql1_X) << "  # mqL1(MX)" << endl
    << right << setw(6) << "42" << setw(19) << m->get_datum(m_ql2_X) << "  # mqL2(MX)" << endl
    << right << setw(6) << "43" << setw(19) << m->get_datum(m_ql3_X) << "  # mqL3(MX)" << endl
    << right << setw(6) << "44" << setw(19) << m->get_datum(m_u_r_X) << "  # muR(MX)" << endl
    << right << setw(6) << "45" << setw(19) << m->get_datum(m_c_r_X) << "  # mcR(MX)" << endl
    << right << setw(6) << "46" << setw(19) << m->get_datum(m_t_r_X) << "  # mtR(MX)" << endl
    << right << setw(6) << "47" << setw(19) << m->get_datum(m_d_r_X) << "  # mdR(MX)" << endl
    << right << setw(6) << "48" << setw(19) << m->get_datum(m_s_r_X) << "  # msR(MX)" << endl
    << right << setw(6) << "49" << setw(19) << m->get_datum(m_b_r_X) << "  # mbR(MX)" << endl;

    return oss.str();
}

string get_slha::mass(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "# mgut=" << m->get_datum(mGUT) << " GeV" << endl
    << "Block MASS   # Mass spectrum" << endl
    << "# PDG code     mass             particle" << endl
    << right << setw(10) << pdg.find(m_w)->second << setw(19) << m->get_datum(m_w) << "   # MW" << endl
    << right << setw(10) << pdg.find(m_h0)->second << setw(19) << m->get_datum(m_h0) << "   # h0" << endl
    << right << setw(10) << pdg.find(m_H0)->second << setw(19) << m->get_datum(m_H0) << "   # H0" << endl
    << right << setw(10) << pdg.find(m_A0)->second << setw(19) << m->get_datum(m_A0) << "   # A0" << endl
    << right << setw(10) << pdg.find(m_Hpm)->second << setw(19) << m->get_datum(m_Hpm) << "   # H+" << endl
    << right << setw(10) << pdg.find(m_g)->second << setw(19) << m->get_datum(m_g) << "   # ~g" << endl
    << right << setw(10) << pdg.find(m_o1)->second << setw(19) << m->get_datum(m_o1) << "   # ~neutralino(1)" << endl
    << right << setw(10) << pdg.find(m_o2)->second << setw(19) << m->get_datum(m_o2) << "   # ~neutralino(2)" << endl
    << right << setw(10) << pdg.find(m_1pm)->second << setw(19) << m->get_datum(m_1pm) << "   # ~chargino(1)" << endl
    << right << setw(10) << pdg.find(m_o3)->second << setw(19) << m->get_datum(m_o3) << "   # ~neutralino(3)" << endl
    << right << setw(10) << pdg.find(m_o4)->second << setw(19) << m->get_datum(m_o4) << "   # ~neutralino(4)" << endl
    << right << setw(10) << pdg.find(m_2pm)->second << setw(19) << m->get_datum(m_2pm) << "   # ~chargino(2)" << endl
    << right << setw(10) << pdg.find(m_d_l)->second << setw(19) << m->get_datum(m_d_l) << "   # ~d_L" << endl
    << right << setw(10) << pdg.find(m_u_l)->second << setw(19) << m->get_datum(m_u_l) << "   # ~u_L" << endl
    << right << setw(10) << pdg.find(m_s_l)->second << setw(19) << m->get_datum(m_s_l) << "   # ~s_L" << endl
    << right << setw(10) << pdg.find(m_c_l)->second << setw(19) << m->get_datum(m_c_l) << "   # ~c_L" << endl
    << right << setw(10) << pdg.find(m_b_1)->second << setw(19) << m->get_datum(m_b_1) << "   # ~b_1" << endl
    << right << setw(10) << pdg.find(m_t_1)->second << setw(19) << m->get_datum(m_t_1) << "   # ~t_1" << endl
    << right << setw(10) << pdg.find(m_e_l)->second << setw(19) << m->get_datum(m_e_l) << "   # ~e_L" << endl
    << right << setw(10) << pdg.find(m_nue_l)->second << setw(19) << m->get_datum(m_nue_l) << "   # ~nue_L" << endl
    << right << setw(10) << pdg.find(m_mu_l)->second << setw(19) << m->get_datum(m_mu_l) << "   # mu_L" << endl
    << right << setw(10) << pdg.find(m_numu_l)->second << setw(19) << m->get_datum(m_numu_l) << "   # ~numu_L" << endl
    << right << setw(10) << pdg.find(m_stau_1)->second << setw(19) << m->get_datum(m_stau_1) << "   # ~stau_1" << endl
    << right << setw(10) << pdg.find(m_nu_tau_l)->second << setw(19) << m->get_datum(m_nu_tau_l) << "   # ~nu_tau_L" <<
    endl
    << right << setw(10) << pdg.find(m_d_r)->second << setw(19) << m->get_datum(m_d_r) << "   # ~d_R" << endl
    << right << setw(10) << pdg.find(m_u_r)->second << setw(19) << m->get_datum(m_u_r) << "   # ~u_R" << endl
    << right << setw(10) << pdg.find(m_s_r)->second << setw(19) << m->get_datum(m_s_r) << "   # ~s_R" << endl
    << right << setw(10) << pdg.find(m_c_r)->second << setw(19) << m->get_datum(m_c_r) << "   # ~c_R" << endl
    << right << setw(10) << pdg.find(m_b_2)->second << setw(19) << m->get_datum(m_b_2) << "   # ~b_2" << endl
    << right << setw(10) << pdg.find(m_t_2)->second << setw(19) << m->get_datum(m_t_2) << "   # ~t_2" << endl
    << right << setw(10) << pdg.find(m_e_r)->second << setw(19) << m->get_datum(m_e_r) << "   # ~e_R" << endl
    << right << setw(10) << pdg.find(m_mu_r)->second << setw(19) << m->get_datum(m_mu_r) << "   # ~mu_R" << endl
    << right << setw(10) << pdg.find(m_stau_2)->second << setw(19) << m->get_datum(m_stau_2) << "   # ~stau_2" << endl;

    return oss.str();
}

string get_slha::alpha(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "# Higgs mixing" << endl
    << "Block alpha   # Effective Higgs mixing parameter" << endl
    << right << setw(25) << m->get_datum(higgs_alpha) << "        # alpha" << endl;

    return oss.str();
}


string get_slha::nmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block nmix  # neutralino mixing matrix" << endl
    << matblock(nmix, 1, 1) << endl
    << matblock(nmix, 1, 2) << endl
    << matblock(nmix, 1, 3) << endl
    << matblock(nmix, 1, 4) << endl
    << matblock(nmix, 2, 1) << endl
    << matblock(nmix, 2, 2) << endl
    << matblock(nmix, 2, 3) << endl
    << matblock(nmix, 2, 4) << endl
    << matblock(nmix, 3, 1) << endl
    << matblock(nmix, 3, 2) << endl
    << matblock(nmix, 3, 3) << endl
    << matblock(nmix, 3, 4) << endl
    << matblock(nmix, 4, 1) << endl
    << matblock(nmix, 4, 2) << endl
    << matblock(nmix, 4, 3) << endl
    << matblock(nmix, 4, 4) << endl;

    return oss.str();
}

string get_slha::umix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block Umix  # Chargino U mixing matrix" << endl
    << matblock(umix, 1, 1) << endl
    << matblock(umix, 1, 2) << endl
    << matblock(umix, 2, 1) << endl
    << matblock(umix, 2, 2) << endl;

    return oss.str();
}

string get_slha::vmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block Vmix  # Chargino V mixing matrix" << endl
    << matblock(vmix, 1, 1) << endl
    << matblock(vmix, 1, 2) << endl
    << matblock(vmix, 2, 1) << endl
    << matblock(vmix, 2, 2) << endl;

    return oss.str();
}

string get_slha::stopmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block stopmix  # stop mixing matrix" << endl
    << matblock(stopmix, 1, 1) << endl
    << matblock(stopmix, 1, 2) << endl
    << matblock(stopmix, 2, 1) << endl
    << matblock(stopmix, 2, 2) << endl;

    return oss.str();
}

string get_slha::sbotmix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block sbotmix  # sbottom mixing matrix" << endl
    << matblock(sbotmix, 1, 1) << endl
    << matblock(sbotmix, 1, 2) << endl
    << matblock(sbotmix, 2, 1) << endl
    << matblock(sbotmix, 2, 2) << endl;

    return oss.str();
}

string get_slha::staumix(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block staumix  # stau mixing matrix" << endl
    << matblock(staumix, 1, 1) << endl
    << matblock(staumix, 1, 2) << endl
    << matblock(staumix, 2, 1) << endl
    << matblock(staumix, 2, 2) << endl;

    return oss.str();
}


string get_slha::gauge(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block gauge Q= " << m->get_datum(gauge_q) << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(gauge_gp) << "   # g'(Q) MSSM DRbar" << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(gauge_g) << "   # g(Q) MSSM DRbar" << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(gauge_g3) << "   # g3(Q) MSSM DRbar" << endl;

    return oss.str();
}

string get_slha::yukawa(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block yu Q= " << m->get_datum(yu_q) << endl
    << "  3  3" << right << setw(19) << m->get_datum(yt) << "   # Yt(Q) MSSM DRbar" << endl
    << "Block yd Q= " << m->get_datum(yu_q) << endl
    << "  3  3" << right << setw(19) << m->get_datum(yb) << "   # Yb(Q) MSSM DRbar" << endl
    << "Block ye Q= " << m->get_datum(yu_q) << endl
    << "  3  3" << right << setw(19) << m->get_datum(ytau) << "   # Ytau(Q) MSSM DRbar" << endl;

    return oss.str();
}

string get_slha::hmix(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block hmix Q= " << m->get_datum(hmix_q) << "    # Higgs mixing parameters" << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_mu) << "    # mu(Q) MSSM DRbar" << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_tb) << "    # tanbeta(Q) MSSM DRbar" << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_vev) << "    # Higgs VEV(Q) MSSM DRbar" << endl;
    oss << right << setw(6) << ++i << setw(19) << m->get_datum(hmix_ma2) << "    # mA^2(Q) MSSM DRbar" << endl;

    return oss.str();
}

string get_slha::msoft(const model *m) const {
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss
    << "Block msoft Q= " << m->get_datum(msoft_q) << "      # MSSM DRbar SUSY breaking parameters" << endl
    << right << setw(6) << "1" << setw(19) << m->get_datum(m1_q) << "      # M_1(Q)" << endl
    << right << setw(6) << "2" << setw(19) << m->get_datum(m2_q) << "      # M_2(Q)" << endl
    << right << setw(6) << "3" << setw(19) << m->get_datum(m3_q) << "      # M_3(Q)" << endl
    << right << setw(6) << "21" << setw(19) << m->get_datum(m_h1sq_q) << "      # MH1^2(Q)" << endl
    << right << setw(6) << "22" << setw(19) << m->get_datum(m_h2sq_q) << "      # MH2^2(Q)" << endl
    << right << setw(6) << "31" << setw(19) << m->get_datum(m_e_l_q) << "      # meL(Q)" << endl
    << right << setw(6) << "32" << setw(19) << m->get_datum(m_mu_l_q) << "      # mmuL(Q)" << endl
    << right << setw(6) << "33" << setw(19) << m->get_datum(m_tau_l_q) << "      # mtauL(Q)" << endl
    << right << setw(6) << "34" << setw(19) << m->get_datum(m_e_r_q) << "      # meR(Q)" << endl
    << right << setw(6) << "35" << setw(19) << m->get_datum(m_mu_r_q) << "      # mmuR(Q)" << endl
    << right << setw(6) << "36" << setw(19) << m->get_datum(m_tau_r_q) << "      # mtauR(Q)" << endl
    << right << setw(6) << "41" << setw(19) << m->get_datum(m_ql1_q) << "      # mqL1(Q)" << endl
    << right << setw(6) << "42" << setw(19) << m->get_datum(m_ql2_q) << "      # mqL2(Q)" << endl
    << right << setw(6) << "43" << setw(19) << m->get_datum(m_ql3_q) << "      # mqL3(Q)" << endl
    << right << setw(6) << "44" << setw(19) << m->get_datum(m_u_r_q) << "      # muR(Q)" << endl
    << right << setw(6) << "45" << setw(19) << m->get_datum(m_c_r_q) << "      # mcR(Q)" << endl
    << right << setw(6) << "46" << setw(19) << m->get_datum(m_t_r_q) << "      # mtR(Q)" << endl
    << right << setw(6) << "47" << setw(19) << m->get_datum(m_d_r_q) << "      # mdR(Q)" << endl
    << right << setw(6) << "48" << setw(19) << m->get_datum(m_s_r_q) << "      # msR(Q)" << endl
    << right << setw(6) << "49" << setw(19) << m->get_datum(m_b_r_q) << "      # mbR(Q)" << endl;

    return oss.str();
}

string get_slha::au(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block au Q= " << m->get_datum(q_au) << endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(au_q) << "      # Au(Q) MSSM DRbar" <<
    endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(ac_q) << "      # Ac(Q) MSSM DRbar" <<
    endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(at_q) << "      # At(Q) MSSM DRbar" <<
    endl;

    return oss.str();
}

string get_slha::ad(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block ad Q= " << m->get_datum(q_ad) << endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(ad_q) << "      # Ad(Q) MSSM DRbar" <<
    endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(as_q) << "      # As(Q) MSSM DRbar" <<
    endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(ab_q) << "      # Ab(Q) MSSM DRbar" <<
    endl;

    return oss.str();
}

string get_slha::ae(const model *m) const {
    int i = 0;
    ostringstream oss;
    oss.setf(ios::scientific);
    oss.precision(8);

    oss << "Block ae Q= " << m->get_datum(q_ae) << endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(ae_q) << "      # Ae(Q) MSSM DRbar" <<
    endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(amu_q) << "      # Amu(Q) MSSM DRbar" <<
    endl;
    oss << right << setw(3) << ++i << setw(3) << i << setw(19) << m->get_datum(atau_q) <<
    "      # Atau(Q) MSSM DRbar" << endl;

    return oss.str();
}


