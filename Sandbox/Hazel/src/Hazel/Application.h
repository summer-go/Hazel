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
namespace Hazel{
class Application {
public:
    Application();
    virtual ~Application();
    void Run();
    void OnEvent(Event& e);
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

private:
    bool OnWindowClose(WindowCloseEvent& e);
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;
};

    Application* CreateApplication();
}



#endif //SANBOX_APPLICATION_H
