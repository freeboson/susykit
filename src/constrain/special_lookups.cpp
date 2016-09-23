/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - special_lookups.cpp:                                                     -
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


#include "constrain/special_lookups.hpp"

#include <cmath>

using namespace std;
using namespace susy_dict;

calc_perms::calc_perms(vector<model>::const_iterator _first, vector<model>::const_iterator _last)
        : first(_first), last(_last) {
    //
}

int calc_perms::operator()(int n) const {
    list<model> l;
    for (vector<model>::const_iterator it = first; it != last; it++) {
        bool isUnique = true;
        for (list<model>::iterator lit = l.begin(); isUnique && lit != l.end(); lit++)
            isUnique = it->hierarchy_cmp(*lit, n);
        if (isUnique)
            l.push_back(*it);
    }
    return l.size();
}

double get_lsq::operator()(const model &m) const {
    std::vector<double> v(8);
    int i = 0;

    v[i++] = m.get_datum(m_u_l);
    v[i++] = m.get_datum(m_u_r);
    v[i++] = m.get_datum(m_c_l);
    v[i++] = m.get_datum(m_c_r);
    v[i++] = m.get_datum(m_d_l);
    v[i++] = m.get_datum(m_d_r);
    v[i++] = m.get_datum(m_s_l);
    v[i++] = m.get_datum(m_s_r);

    return *(std::min_element(v.begin(), v.end()));
}

double get_lsl::operator()(const model &m) const {
    std::vector<double> v(4);
    int i = 0;

    v[i++] = m.get_datum(m_mu_l);
    v[i++] = m.get_datum(m_mu_r);
    v[i++] = m.get_datum(m_e_l);
    v[i++] = m.get_datum(m_e_r);

    return *(std::min_element(v.begin(), v.end()));
}

double get_RGG_approx::operator()(const model &m) const {
    // these are necessary to compute m_\tau(Q)
    double tau_yukawa = m.get_datum(ytau); // Y_\tau(Q)
    double full_vev = m.get_datum(hmix_vev); // v(Q) = \sqrt{v_1(Q)^2 + v_2(Q)^2}
    double tanbeta = m.get_datum(hmix_tb); // \tan\beta(Q) = v_2(Q)/v_1(Q)
    double cosbeta = 1.0 / sqrt(1 + tanbeta * tanbeta);

    double mu = m.get_datum(hmix_mu);
    double m_stau1 = m.get_datum(m_stau_1);

    double m_tau = tau_yukawa * (full_vev * cosbeta); // Y_\tau v\cos\beta

#if 0    // this was using Eqn (5) from arXiv:1207.6393
    // these are to comptue \sin{2\theta_{\stau}}
    double tau_trilinear = m.get_datum(atau_q);
    double m_stau2 = m.get_datum(m_stau_2);
    double sin2theta_stau =	  (2*m_tau*(tau_trilinear-mu*tanbeta))  // numerator
                / (m_stau1_sq - m_stau2*m_stau2); // denominator
#else
    // since we have the stau mixing matrix, we can get \theta_\stau directly
    double m_stauL = m.get_datum(m_tau_l_q);
    double m_stauR = m.get_datum(m_tau_r_q);

    // if mL < mR, SOFTSUSY uses rot2d()
    // if ml >= mR, SOFTSUSY uses rot2dtwist()
    // the upshot is that in the first case, the (1,2)
    // element is \cos(\theta_\stau), in the second it
    // is \sin(\theta_\stau)

    double theta_stau = (m_stauL < m_stauR) ?
                        acos(m.get_datum(staumix_12)) :
                        asin(m.get_datum(staumix_12));

#endif

    // this is Eqn (7) from arXiv:1207.6393
    double rgg_sqrt = (1.0 + 0.025 * abs(m_tau * mu * tanbeta * sin(2 * theta_stau)) / (m_stau1 * m_stau1));
    // the result is rgg_sqrt^2

    return rgg_sqrt * rgg_sqrt;
}





