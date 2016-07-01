
#pragma once
#ifndef PRIORS_HPP
#define PRIORS_HPP

namespace priors {

    class prior {

    public:

        prior(double _min, double _max)
                : min(_min), max(_max) {
            diff = max - min;
        }

        virtual double map(double variate) const = 0;

        double get_min() const { return min; }

        double get_max() const { return max; }

        double get_diff() const { return diff; }

    private:

        double min, max;
        double diff;

    };

    class linear : public prior {

    public:
        linear(double _min, double _max)
                : prior(_min, _max) {

        }

        double map(double variate) const;
    };

    class log : public prior {

    public:
        log(double _min, double _max)
                : prior(_min, _max) {

        }

        double map(double variate) const;
    };

}


#endif


