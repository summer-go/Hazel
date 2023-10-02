//
// Created by baidu on 2023/9/2.
//
#include <functional>
#include "Application.h"
#include "ApplicationEvent.h"
#include "Log.h"
#include "ImGui/ImGuiLayer.h"
#include <glad/glad.h>
#include <iostream>

namespace Hazel{
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:         return GL_FLOAT;
            case ShaderDataType::Float2:        return GL_FLOAT;
            case ShaderDataType::Float3:        return GL_FLOAT;
            case ShaderDataType::Float4:        return GL_FLOAT;
            case ShaderDataType::Mat3:          return GL_FLOAT;
            case ShaderDataType::Mat4:          return GL_FLOAT;
            case ShaderDataType::Int:           return GL_INT;
            case ShaderDataType::Int2:          return GL_INT;
            case ShaderDataType::Int3:          return GL_INT;
            case ShaderDataType::Int4:          return GL_INT;
            case ShaderDataType::Bool:          return GL_BOOL;
        }
        HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }


    Application::Application() {
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        // 绑定类成员函数，需要传this
//        Window::EventCallbackFn fn = std::bind(&Application::OnEvent, this, std::placeholders::_1);
//        m_Window->SetEventCallback(fn);
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);


        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 7] = {
                -0.5f, -0.5, 0.0, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        unsigned int indices[3] = {0, 1, 2};
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

        {
            BufferLayout layout = {
                    {ShaderDataType::Float3, "a_Position"},
                    {ShaderDataType::Float4, "a_Color"},
            };
            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.GetComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                  element.Normallized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  (const void*)element.Offset);
            index++;
        }

        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            out vec3 v_Position;
            out vec4 v_Color;
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
        HZ_CORE_TRACE("{0}", e.ToString());

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
        WindowResizeEvent e(1200, 720);
        if (e.IsInCategory(EventCategoryApplication)) {
            HZ_TRACE(e.ToString());
        }

        if (e.IsInCategory(EventCategoryInput)) {
            HZ_TRACE(e.ToString());
        }

        while(m_Running) {
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
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