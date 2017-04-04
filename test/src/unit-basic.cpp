/*

    ----------------------------------------------------------------------------
    -                                                                          -
    - Copyright 2011-2016 Sujeet Akula                                         -
    - sujeet@freeboson.org                                                     -
    -                                                                          -
    - unit-basic.cpp:                                                          -
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


#include <string>
#include <sstream>

#include "constrain/model.hpp"
#include "constrain/parse.hpp"

#include "catch/catch.hpp"

using namespace std;

const static string mline = "NUSUGRA-G	1.28066266e+16	1.27925000e+02	1.16637000e-05	1.18400000e-01	9.11876000e+01	4.20000000e+00	1.73100000e+02	1.77699000e+00	4.31010000e+01	1.00000000e+00	3.89240000e+03	1.54650000e+03	4.73800000e+03	3.80907360e+03	3.80907360e+03	3.80907360e+03	8.63830881e+06	8.63830881e+06	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	2.93910000e+03	8.03779080e+01	1.25024686e+02	4.00786765e+03	4.00783438e+03	4.00872329e+03	9.43634770e+03	1.25347322e+03	1.74252767e+03	1.25362680e+03	-4.40475741e+03	4.40545757e+03	4.40586101e+03	8.45530407e+03	8.45507768e+03	8.45516267e+03	8.45493626e+03	7.63579368e+03	7.26844778e+03	3.14282509e+03	3.14151917e+03	3.14170322e+03	3.14056102e+03	2.58467381e+03	2.81387932e+03	8.44235975e+03	8.47388222e+03	8.44209138e+03	8.47386605e+03	7.99033342e+03	7.64398300e+03	3.26398520e+03	3.26213691e+03	2.83124599e+03	-2.39084832e-02	-5.74095230e-04	9.99913055e-01	-4.97688545e-03	-1.21976933e-02	9.99798597e-01	8.36777557e-04	9.46516519e-03	1.76769615e-02	-1.92007441e-02	1.21305405e-02	7.07010193e-01	7.06838573e-01	5.81088248e-03	-5.10220406e-03	7.07122499e-01	-7.07048777e-01	9.99646332e-01	-2.65934546e-02	2.65934546e-02	9.99646332e-01	9.99965448e-01	-8.31283978e-038.31283978e-03	9.99965448e-01	1.21467387e-01	9.92595423e-01	9.92595423e-01	-1.21467387e-01	9.97675019e-01	6.81509830e-02	-6.81509830e-02	9.97675019e-01	2.60022534e-01	9.65602548e-01	9.65602548e-01	-2.60022534e-01	7.26916134e+03	3.66822359e-01	6.34672660e-01	9.66758513e-01	7.26916134e+03	7.91277999e-01	4.83359064e-01	4.45641855e-01	7.26916134e+03	4.38433332e+03	4.18663793e+01	2.42048801e+02	2.11714207e+07	7.26916134e+03	1.76469658e+03	1.18130739e+03	9.22480755e+03	-2.74233229e+06	-1.82475387e+07	3.13304114e+03	3.13208261e+03	2.82415788e+03	3.25091359e+03	3.24905868e+03	2.61991584e+03	8.20275317e+03	8.20260997e+03	7.43959154e+03	8.23555800e+03	8.23554160e+03	7.10094330e+03	8.20386383e+03	8.20359161e+03	7.78495534e+03	7.26916134e+03	-4.98991730e+03	-4.98988165e+03	-4.59300321e+03	7.26916134e+03	-5.32687210e+03	-5.32677877e+03	-5.07403203e+03	7.26916134e+03	2.62543635e+03	2.62383147e+03	2.10836845e+03";

const static string mline_merged = mline + "    1	5.79725469e-07	6.70689943e-11	3.30652483e-04	3.09454390e-09	9.94320293e-01	3.97022798e-02	3.07799605e-11	4.04605487e-09	3.13031541e-11	4.11073283e-09" + " 1 0 0 0 0 0 0 0 0 0 0";

TEST_CASE("constrain") {
    SECTION("model_parser") {
        model_parser mp;
        model m = mp.parse(mline, false);
        CHECK(abs(m.get_datum(susy_dict::m_h0) - 125.024686) < 1e-7);
    }

    SECTION("stream_model_parser") {
        stringstream ss(mline);
        stream_model_parser smp(&ss, false);
        model m = smp();
        CHECK(abs(m.get_datum(susy_dict::m_h0) - 125.024686) < 1e-7);
    }

    SECTION("model_parser (merged)") {
        model_parser mp;
        model m = mp.parse(mline_merged, true);
        CHECK(abs(m.get_datum(susy_dict::m_h0) - 125.024686) < 1e-7);
    }

    SECTION("stream_model_parser (merged)") {
        stringstream ss(mline_merged);
        stream_model_parser smp(&ss, true);
        model m = smp();
        CHECK(abs(m.get_datum(susy_dict::m_h0) - 125.024686) < 1e-7);
    }

}
