#pragma once
#include <map>
#include <array>
#include <string>
#include <vector>
#include <complex>
#include <iostream>
#include <typeinfo>

#include <mpi.h>

#include "global.hpp"
#include "iterators.hpp"
#include "darray.hpp"
#include "mpi.hpp"

// DEFAULT CONFIGURATION OPTIONS
#ifndef DARRAY_CHECKBOUNDS
#define DARRAY_CHECKBOUNDS false
#endif

