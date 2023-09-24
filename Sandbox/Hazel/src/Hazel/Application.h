//
// Created by baidu on 2023/9/2.
//

#ifndef SANBOX_APPLICATION_H
#define SANBOX_APPLICATION_H

#include "Event.h"
#include "ApplicationEvent.h"
#include <memory>
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
namespace Hazel{
class Application {
public:
    Application();
    virtual ~Application();
    void Run();
    void OnEvent(Event& e);
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() {
        return *m_Window;
    }
    inline static Application& Get() {
        return *s_Instance;
    }

private:
    bool OnWindowClose(WindowCloseEvent& e);
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;
    unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
    unsigned int shaderProgram;
private:
    static Application* s_Instance;
};

    Application* CreateApplication();
}



#endif //SANBOX_APPLICATION_H
