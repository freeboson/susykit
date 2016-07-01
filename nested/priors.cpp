#include <cmath>
#include "priors.hpp"

using namespace priors;
using namespace std;

double linear::map(double variate) const {
    return get_min() + variate * get_diff();
}

double log::map(double variate) const {
    return pow(10.0, get_min() + variate * get_diff());
}

