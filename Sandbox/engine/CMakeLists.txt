cmake_minimum_required(VERSION 2.8)
project(engine)
set(SRC_LIST src/Test.cpp)
add_library(engine STATIC ${SRC_LIST})