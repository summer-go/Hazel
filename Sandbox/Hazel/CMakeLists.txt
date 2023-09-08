cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

project(hazel)
set(SRC_LIST
        src/Hazel/Application.cpp
        src/Hazel/Log.cpp
        src/Hazel/Platform/Windows/WindowsWindow.cpp
        src/Hazel/glad.c
        src/Hazel/Layer.h src/Hazel/Layer.cpp
        src/Hazel/LayerStack.cpp src/Hazel/LayerStack.h)


add_library(hazel STATIC ${SRC_LIST})
add_subdirectory(vendor/GLFW)

target_link_libraries(${PROJECT_NAME}  glfw)
target_include_directories(${PROJECT_NAME} PUBLIC
        vendor/spdlog/include
        vendor/GLFW/include
        src/Hazel/Core
        src/Hazel/Events
        src/Hazel
        src/Hazel/Platform/include
        )

LINK_DIRECTORIES(/usr/local/lib)
