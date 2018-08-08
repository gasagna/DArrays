#pragma once

namespace DArrays {

// ========================================================================== //
// DArray
template <typename T>
class DArray {
private:
    std::array<int, 3> _g_arr_size; // global array size
    std::array<int, 3> _l_arr_size; // local array size
    MPI_Datatype       _block_type; // mpi type for data transpose
    int                 _comm_size; // number of processors
    int                 _comm_rank; // rank of this processor
    int                  _decstate; // current domain decomposition
    std::vector<T>           _data; // actual data
    std::vector<T>           _buff; // buffer for local transposition
    MPI_Comm                 _comm; // communicator

    // ====================================================================== //
    // bound checking
    inline void _checkbounds(int i, int j, int k) {
        if ((i < 0 or i >= _l_arr_size[0]) or
            (j < 0 or j >= _l_arr_size[1]) or
            (k < 0 or k >= _l_arr_size[2]))
            throw std::out_of_range("Out of range");
    }

    inline void _checkbounds(int i) {
        if (i < 0 or i >= _l_arr_size[0]*_l_arr_size[1]*_l_arr_size[2])
            throw std::out_of_range("Out of range");
    }

    // init function used in constructor
    inline void _create_l_arr_size() {
        // init from full size
        _l_arr_size = _g_arr_size;

        // check data can be partitioned equally along transposition directions
        div_t divrem;
        for (auto dim : {0, 1}) {
            divrem = div(_g_arr_size[dim], _comm_size);
            if (divrem.rem != 0)
                throw std::invalid_argument("incompatible global array "
                    "size and communicator size");
        }
        // partition the data along the direction of current decomposition
        _l_arr_size[_decstate] = _g_arr_size[_decstate]/_comm_size;
    }

    // init function used in constructor
    inline void _create_subarray_type() {
        // always start here
        std::array<int, 3> origin {0, 0, 0};
        std::array<int, 3> sasize {0, 0, 0};

        // construct size of the subarray used for data transfer
        sasize = _l_arr_size;
        sasize[!_decstate] = _g_arr_size[!_decstate]/_comm_size;

        // construct temporary type
        MPI_Datatype tmp_type;
        MPI_Type_create_subarray(3,
                                 _l_arr_size.data(),
                                 sasize.data(),
                                 origin.data(),
                                 MPI_ORDER_FORTRAN, 
                                 MPI_TYPE_MAP[typeid(T).name()], &tmp_type);

        // then resize extent
        MPI_Type_create_resized(tmp_type,
                                0, sasize[0]*sizeof(T), &_block_type);

        // remember to free this at destruction
        MPI_Type_commit(&_block_type);
    }

    // ====================================================================== //
    // UTILITIES FOR LOCAL TRANSPOSITION
    // void __local_transpose_01() {
    //     for (auto k : LinRange(_local_array_size[2])) {
    //         int n = 0
    //         for (auto j :  LinRange(_local_array_size[1]))
    //             for (auto m : LinRange(_comm_size))
    //                 for (auto i :  LinRange(_local_array_size[0]))
    //                     _buff[n++] = (*this)(i, j+m*_local_array_size[1], k);
    //         std::copy(_buff.begin(), _buff.end(), _data.begin()+k*_buff.size());
    //     }
    // }

    // void __local_transpose_10() {
    //     for (auto k : LinRange(_local_array_size[2])) {
    //         int n = 0
    //         for (auto i :  LinRange(_local_array_size[0]))
    //             for (auto m : LinRange(_comm_size))
    //                 for (auto j :  LinRange(_local_array_size[1]))
    //                     _buff[n++] = (*this)(i+m*_local_array_size[0], j, k);
    //         std::copy(_buff.begin(), _buff.end(), _data.begin()+k*_buff.size());
    //     }
    // }

public:
    // ====================================================================== //
    // constructor/destructor
    DArray() = delete;

    DArray(const MPI_Comm&                 comm,
           const std::array<int, 3>& g_arr_size,
           int                         decstate)
        : _g_arr_size (g_arr_size)
        , _comm       (comm      ) {
            // check decomposition state is zero or one
            if ( !(decstate == 0 || decstate == 1) )
                throw std::invalid_argument("decstate must be 0 or 1");

            // then assign
            _decstate = decstate;
            
            // get communicator size and rank
            MPI_Comm_size(comm, &_comm_size);
            MPI_Comm_rank(comm, &_comm_rank);

            // define size of local array
            _create_l_arr_size();

            // construct subarray
            _create_subarray_type();

            // allocate memory buffer
            _data.reserve(_l_arr_size[0]*_l_arr_size[1]*_l_arr_size[2]);

            // this is used for the local transposition
            _buff.reserve(_l_arr_size[0]*_l_arr_size[1]);
    }

    ~DArray() {
        // MPI_Type_free(&_block_type);
    }

    // ====================================================================== //
    // linear indexing
    inline T& operator [] (int i) {
        #if DARRAY_CHECKBOUNDS
            _checkbounds(i);
        #endif
        return _data[i];
    }

    // cartesian indexing
    inline T& operator () (int i, int j, int k) {
        #if DARRAY_CHECKBOUNDS
            _checkbounds(i, j, k);
        #endif
        return _data[i + _l_arr_size[0]*(j + _l_arr_size[1]*k)];
    }

    // ====================================================================== //
    // get size along dimension dim
    inline int size(int dim) const {
        #if DARRAY_CHECKBOUNDS
            if (dim >= 3 or dim < 0)
                throw std::out_of_range("dimension out of range");
        #endif
        return _l_arr_size[dim];
    }

    // ====================================================================== //
    // transpose dimensions
    // void transpose() {

    //     // call to mpi function: easy peasy
    //     MPI_Alltoall(_data.data(), 1, _block_type,
    //                  _data.data(), 1, _block_type, _comm);

    //     // perform local data transpose
    //     switch (_decstate) {
    //         case 0 : __local_transpose_01(); break;
    //         case 1 : __local_transpose_10(); break;
    //     }

    //     // permute local and global dimensions
    //     std::swap(_l_arr_size[0], _l_arr_size[1]);
    //     std::swap(_g_arr_size[0], _g_arr_size[1]);

    //     // then finally flip the state
    //     _decstate = !_decstate;
    // }
};

}