cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)

project(hazel)


file (GLOB SRC_LIST_IMGUI
        vendor/imgui/*.h
        vendor/imgui/*.cpp
)

set(SRC_LIST_IMGUI_BACKENDS
        vendor/imgui/backends/imgui_impl_opengl3.h
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/imgui/backends/imgui_impl_opengl3_loader.h
        vendor/imgui/backends/imgui_impl_glfw.h
        vendor/imgui/backends/imgui_impl_glfw.cpp
)

set(SRC_LIST
        src/Hazel/Application.cpp
        src/Hazel/Log.cpp
        src/Hazel/Platform/Windows/WindowsWindow.cpp
        src/Hazel/glad.c
        src/Hazel/Layer.h src/Hazel/Layer.cpp
        src/Hazel/LayerStack.cpp src/Hazel/LayerStack.h
        src/Hazel/ImGui/ImGuiLayer.cpp
        src/Hazel/ImGui/ImGuiLayer.h
        src/Hazel/Input.h
        src/Hazel/Platform/Windows/WindowsInput.cpp
        src/Hazel/Platform/Windows/WindowsInput.h
        src/Hazel/KeyCodes.h
        src/Hazel/MouseButtonCodes.h
)


add_library(hazel STATIC ${SRC_LIST} ${SRC_LIST_IMGUI} ${SRC_LIST_IMGUI_BACKENDS})
add_subdirectory(vendor/GLFW)

target_link_libraries(${PROJECT_NAME}  glfw)
target_include_directories(${PROJECT_NAME} PUBLIC
        vendor/spdlog/include
        vendor/GLFW/include
        vendor/imgui
        vendor/imgui/backends
        vendor/glm
        src/Hazel/Core
        src/Hazel/Events
        src/Hazel
        src/Hazel/Platform/include
        )

LINK_DIRECTORIES(/usr/local/lib)
