#pragma once

namespace DArrays::MPI {

// ===================================================================== //
// initialize/finalize mpi session
inline void Initialize() {
    MPI_Init(nullptr, nullptr);
}

inline void Finalize() {
    MPI_Finalize();
}

}