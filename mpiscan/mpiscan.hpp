#include <fstream>
#include <string>
#include <memory>
#include <random>
#include <tuple>

#include "softsusy_interface.hpp"


class point_builder {
public:

    point_builder(int pars_end);

    std::string mpi_setup(std::fstream *writer);

    std::mt19937_64 &gen() { return *gen_ptr; }

    long double get_time() const { return (curr_time() - time); }

    virtual softsusy_opts operator()() = 0;

    virtual bool filter_point(const model &m) {
        return false; //default -- don't filter
    }

    ~point_builder();

    DoubleVector pars;

private:

    long double curr_time() const;

    std::shared_ptr<std::mt19937_64> gen_ptr;
    long double time;

};


