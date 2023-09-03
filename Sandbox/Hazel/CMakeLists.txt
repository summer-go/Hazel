cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project(hazel)
set(SRC_LIST
        src/Hazel/Application.cpp
        src/Hazel/Log.cpp)
add_library(hazel STATIC ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC
                            vendor/spdlog/include
                            src/Hazel/Core
                            Src/Hazel/Events)