//
// Created by baidu on 2023/9/5.
//

#ifndef SANBOX_WINDOW_H
#define SANBOX_WINDOW_H

#include "../Core.h"
#include "Event.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel {
    struct WindowProps {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = (std::string)("Hazel Engine"),
                    unsigned int width = 1280,
                    unsigned int height = 720)
                    : Title(title), Width(width), Height(height) {
        }

    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        virtual ~Window() = default;
        virtual void OnUpdate() = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        virtual GLFWwindow* GetNativeWindow(){return nullptr;};

        static Window* Create(const WindowProps& props = WindowProps());
    };

}

#endif //SANBOX_WINDOW_H
