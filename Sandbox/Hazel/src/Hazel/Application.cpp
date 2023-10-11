//
// Created by baidu on 2023/9/2.
//
#include <functional>
#include "Application.h"
#include "ApplicationEvent.h"
#include "Log.h"
#include "ImGui/ImGuiLayer.h"

namespace Hazel{
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;
        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    Application::~Application() {

    }

    void Application::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
//        HZ_CORE_TRACE("{0}", e.ToString());

        for (auto it = m_LayerStack.end();  it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled) {
                break;
            }
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while(m_Running) {
            float time = glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(timestep);
            }
            m_ImGuiLayer->Begin();
            for(Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            m_Window->OnUpdate();
        }
    }

}