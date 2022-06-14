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

# include the file containing the sanitizer option
include(Sanitizers.cmake)

# include directory
include_directories(include)

# add executable
add_executable(test_interval app/test_interval.cpp)
target_link_libraries(test_interval Catch2::Catch2)