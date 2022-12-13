cmake_minimum_required(VERSION 3.23)
project(BigInt-C-)
set(CMAKE_CXX_STANDARD 20)
add_executable(BigInt-C- main.cpp Utility.cpp Binary.cpp BigInt.cpp BigInt.h)