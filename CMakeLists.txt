cmake_minimum_required(VERSION 3.10)
project(ns-distro VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(VALID_ARCHITECTURES "x86_64" "aarch64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -Wall -O2")

if(ARCHITECTURE STREQUAL "aarch64")
    set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
elseif(ARCHITECTURE STREQUAL "x86_64")
    set(CMAKE_CXX_COMPILER "x86_64-linux-gnu-g++")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(
    switch-distro 
    src/main.cpp 
    src/terminal.cpp 
    src/environment.cpp
)