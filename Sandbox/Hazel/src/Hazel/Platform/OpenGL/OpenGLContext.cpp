//
// Created by xiatian05 on 2023/9/24.
//

#include "OpenGLContext.h"
#include "Core/Base.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

namespace Hazel {
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle):m_WindowHandle(windowHandle) {
        HZ_CORE_ASSERT(windowHandle, "Window handle is null");
    }

    void OpenGLContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        HZ_CORE_INFO("OpenGL Info:");
        HZ_CORE_INFO(" Vendor: {0}", (char*)glGetString(GL_VENDOR));
        HZ_CORE_INFO(" Renderer: {0}", (char*)glGetString(GL_RENDERER));
        HZ_CORE_INFO(" Version: {0}", (char*)glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(m_WindowHandle);
    }
}