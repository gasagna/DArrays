#include <complex>
#include <string>
#include <map>

#include <mpi.h>
#include "global.hpp"

namespace DArrays {

// now fill the type map
std::map<std::string, MPI_Datatype> MPI_TYPE_MAP {
    {typeid(               float).name(),          MPI_FLOAT},
    {typeid(              double).name(),         MPI_DOUBLE},
    {typeid( std::complex<float>).name(),        MPI_COMPLEX},
    {typeid(std::complex<double>).name(), MPI_DOUBLE_COMPLEX},
};

}