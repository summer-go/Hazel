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
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthographicCamera.h"
#include "Core.h"

int main(int argc, char** argv);

namespace Hazel{
class Application {
public:
    Application();
    virtual ~Application();
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
    // 优化，将Run()函数放到private下，不对外公开暴露
    // main通过friend的形式调用Run();
    void Run();
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
private:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0;

private:
    static Application* s_Instance;
    friend int ::main(int argc, char** argv);
};

    Application* CreateApplication();
}



#endif //SANBOX_APPLICATION_H
