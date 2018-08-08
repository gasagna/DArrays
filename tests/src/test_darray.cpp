#include "DArrays.hpp"
#include <catch.hpp>
#include <array>
#include <iostream>

// import all
using namespace DArrays;
using namespace DArrays::Iterators;

TEST_CASE("DArrays", "all tests") {

    SECTION("restrict array size") {
        // global array size
        std::array<int, 3> g_arr_size = {4, 5, 6};

        // define array: 
        // see https://github.com/catchorg/Catch2/issues/536#issuecomment-155411223
        REQUIRE_THROWS( DArray<double>(MPI_COMM_WORLD, g_arr_size, 0) );
    }

    SECTION("indexing") {
        // global array size
        std::array<int, 3> g_arr_size = {2, 2, 3};

        // define array
        DArray<double> a(MPI_COMM_WORLD, g_arr_size, 0);

        // check sizes
        REQUIRE( a.size(0) == 1 );
        REQUIRE( a.size(1) == 2 );
        REQUIRE( a.size(2) == 3 );

        // check out of bounds
        REQUIRE_THROWS( a(-1,  0,  0) );
        REQUIRE_THROWS( a( 0, -1,  0) );
        REQUIRE_THROWS( a( 0,  0, -1) );
        REQUIRE_THROWS( a( 1,  0,  0) );
        REQUIRE_THROWS( a( 0,  2,  0) );
        REQUIRE_THROWS( a( 0,  0,  3) );
        REQUIRE_THROWS( a[-1] );
        REQUIRE_THROWS( a[ 6] );

        // verify indexing and fortran ordering
        for (auto i : LinRange(1*2*3))
            a[i] = i;
        
        REQUIRE( a(0,  0,  0) == 0);
        REQUIRE( a(0,  1,  0) == 1);
        REQUIRE( a(0,  0,  1) == 2);
        REQUIRE( a(0,  1,  1) == 3);
        REQUIRE( a(0,  0,  2) == 4);
        REQUIRE( a(0,  1,  2) == 5);
    }
}
