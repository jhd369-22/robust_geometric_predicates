cmake_minimum_required(VERSION 3.23)

# set the project name
project(test_code LANGUAGES CXX)

# set Debug build type
set(CMAKE_BUILD_TYPE Debug)

# set c++ standard 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find catch2
find_package(Catch2 REQUIRED)

find_package(CGAL REQUIRED)

# include the file containing the sanitizer option
include(Sanitizers.cmake)

# include directory
include_directories(include)

# Note that, as of the time of this writing, neither
# GCC nor Clang support the “STDC FENV_ACCESS ON” pragma.
# In the case of GCC, a workaround for this problem is
# to add the “-frounding-math” flag to the compiler when
# compiling any code that explicitly changes rounding modes
# (e.g., any code that uses interval arithmetic).
# This should be done by adding the preceding flag to
# the `CMAKE_CXX_FLAGS` variable.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math")

# add executable
add_executable(test_interval app/test_interval.cpp)
target_link_libraries(test_interval Catch2::Catch2)

add_executable(test_kernel app/test_kernel.cpp)
target_link_libraries(test_kernel Catch2::Catch2 CGAL::CGAL)