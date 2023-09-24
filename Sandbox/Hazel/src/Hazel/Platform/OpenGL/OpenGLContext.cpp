//
// Created by xiatian05 on 2023/9/24.
//

#include "OpenGLContext.h"
#include "Core/Base.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel {
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle):m_WindowHandle(windowHandle) {
        HZ_CORE_ASSERT(windowHandle, "Window handle is null");
    }

    void OpenGLContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(m_WindowHandle);
    }
}