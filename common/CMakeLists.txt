cmake_minimum_required(VERSION 3.9...3.31)
project(aoc LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "CMakeFiles/.*")
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "*.hpp")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

add_executable(aoc ${SOURCES})

if(OMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(aoc PUBLIC OpenMP::OpenMP_CXX)
endif()
