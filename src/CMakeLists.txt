cmake_minimum_required(VERSION 3.22)
project(MLP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE SOURCES
        "Neuro/*.cc")

file(GLOB_RECURSE HEADERS
        "Neuro/*.h")

include_directories(${HEADERS})

add_executable(MLP.out
        ${SOURCES})
