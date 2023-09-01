cmake_minimum_required(VERSION 2.8)
project(hazel)
set(SRC_LIST src/Hazel/Application.cpp)
add_library(hazel STATIC ${SRC_LIST})