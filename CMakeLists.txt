cmake_minimum_required(VERSION 3.0.0)
project(DArrays)

# include directories
include_directories(include)

# add compiler flags
set(CXX_FLAGS "--std=c++1z -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

# link to mpi libs
set(CMAKE_CXX_STANDARD_LIBRARIES  -lmpi)

# add files
file(GLOB FILES "src/*.cpp")

# create executable
add_library(DArrays SHARED ${FILES})