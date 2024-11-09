cmake_minimum_required(VERSION 3.11)

project(qgm_cpp_example VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)


# Build type
set(CMAKE_BUILD_TYPE Release)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")


# Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIRS})


add_subdirectory(fracgm)
message(STATUS "Building source done")


add_executable(fracgm_example fracgm_example.cpp)
target_link_libraries(fracgm_example PRIVATE FracGM::fracgm)