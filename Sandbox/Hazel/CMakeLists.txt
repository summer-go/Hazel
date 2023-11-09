cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)

project(hazel)


file (GLOB SRC_LIST_STB_IMAGE
        vendor/stb_image/*.h
        vendor/stb_image/*.cpp
)
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
        src/Hazel/Core/Application.cpp
        src/Hazel/Core/Log.cpp
        src/Hazel/Platform/Windows/WindowsWindow.cpp
        src/Hazel/glad.c
        src/Hazel/Core/Layer.h src/Hazel/Core/Layer.cpp
        src/Hazel/Core/LayerStack.cpp src/Hazel/Core/LayerStack.h
        src/Hazel/ImGui/ImGuiLayer.cpp
        src/Hazel/ImGui/ImGuiLayer.h
        src/Hazel/Core/Input.h
        src/Hazel/Platform/Windows/WindowsInput.cpp
        src/Hazel/Platform/Windows/WindowsInput.h
        src/Hazel/Core/KeyCodes.h
        src/Hazel/Core/MouseButtonCodes.h
        src/Hazel/Renderer/GraphicsContext.h
        src/Hazel/Platform/OpenGL/OpenGLContext.cpp
        src/Hazel/Platform/OpenGL/OpenGLContext.h
        src/Hazel/Renderer/Shader.cpp
        src/Hazel/Renderer/Shader.h
        src/Hazel/Renderer/Renderer.cpp
        src/Hazel/Renderer/Renderer.h
        src/Hazel/Renderer/Buffer.cpp
        src/Hazel/Renderer/Buffer.h
        src/Hazel/Platform/OpenGL/OpenGLBuffer.cpp
        src/Hazel/Platform/OpenGL/OpenGLBuffer.h
        src/Hazel/Renderer/VertexArray.cpp
        src/Hazel/Renderer/VertexArray.h
        src/Hazel/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Hazel/Platform/OpenGL/OpenGLVertexArray.h
        src/Hazel/Renderer/RendererAPI.cpp
        src/Hazel/Renderer/RendererAPI.h
        src/Hazel/Platform/OpenGL/OpenGLRendererAPI.cpp
        src/Hazel/Platform/OpenGL/OpenGLRendererAPI.h
        src/Hazel/Renderer/RenderCommand.cpp
        src/Hazel/Renderer/RenderCommand.h
        src/Hazel/Renderer/OrthographicCamera.cpp
        src/Hazel/Renderer/OrthographicCamera.h
        src/Hazel/Core/Timestep.h
        src/Hazel/Platform/OpenGL/OpenGLShader.cpp
        src/Hazel/Platform/OpenGL/OpenGLShader.h
        vendor/stb_image/stb_image.cpp
        vendor/stb_image/stb_image.h
        src/Hazel/Renderer/Texture.cpp
        src/Hazel/Renderer/Texture.h
        src/Hazel/Platform/OpenGL/OpenGLTexture.cpp
        src/Hazel/Platform/OpenGL/OpenGLTexture.h
        src/Hazel/OrthographicCameraController.cpp
        src/Hazel/OrthographicCameraController.h
        src/Hazel/Renderer/Renderer2D.cpp
        src/Hazel/Renderer/Renderer2D.h
        src/Hazel/Debug/Instrumentor.h
)


add_library(hazel STATIC ${SRC_LIST} ${SRC_LIST_IMGUI} ${SRC_LIST_IMGUI_BACKENDS} ${SRC_LIST_STB_IMAGE})
add_subdirectory(vendor/GLFW)

target_link_libraries(${PROJECT_NAME}  glfw)
target_include_directories(${PROJECT_NAME} PUBLIC
        vendor/spdlog/include
        vendor/GLFW/include
        vendor/imgui
        vendor/imgui/backends
        vendor/glm
        vendor/stb_image
        src/Hazel/Core
        src/Hazel/Events
        src/Hazel
        src/Hazel/Platform/include
        )

LINK_DIRECTORIES(/usr/local/lib)
