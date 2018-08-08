#pragma once

namespace DArrays {

// ===================================================================== //
// map between c++ types and MPI primitive types
extern std::map<std::string, MPI_Datatype> MPI_TYPE_MAP;

}