//
// Created by baidu on 2023/9/1.
//

//#include "Test.h"
#include <Hazel.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "imgui.h"

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}


class ExampleLayer : public Hazel::Layer {
public:
    ExampleLayer() : Layer("Example"),m_Camera(-1.6f, 1.6f, -0.9f, 0.9f){
        auto cam = camera(1.0, glm::vec2());

        //-----------------三角形----------------------
        m_VertexArray.reset(Hazel::VertexArray::Create());
        float vertices[3 * 7] = {
                -0.5f, -0.5, 0.0, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        std::shared_ptr<Hazel::VertexBuffer> vertexBuffer(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
                {Hazel::ShaderDataType::Float3, "a_Position"},
                {Hazel::ShaderDataType::Float4, "a_Color"},
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Hazel::IndexBuffer> indexBuffer(Hazel::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

        m_VertexArray->SetIndexBuffer(indexBuffer);

        // -----------------矩形--------------------
        m_SquareVA.reset(Hazel::VertexArray::Create());
        // 注意矩形，顶点按照逆时针排列
        float squareVertices[3*4] = {
                -0.75f, -0.75f, 0.0f,
                0.75f, -0.75f, 0.0f,
                0.75f, 0.75f, 0.0f,
                -0.75f, 0.75f, 0.0f
        };
        std::shared_ptr<Hazel::VertexBuffer> squareVB(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
                                    {Hazel::ShaderDataType::Float3, "a_Position"}
                            });
        m_SquareVA->AddVertexBuffer(squareVB);
        uint32_t squareIndices[6] = {0,1,2,2,3,0};
        std::shared_ptr<Hazel::IndexBuffer> squareIB;
        squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices)/sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);


        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_Shader = std::make_shared<Hazel::Shader>(vertexSrc, fragmentSrc);

        std::string blueShaderVertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;

            void main() {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core
            layout(location=0) out vec4 color;
            in vec3 v_Position;

            void main() {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader = std::make_shared<Hazel::Shader>(blueShaderVertexSrc, blueShaderFragmentSrc);
    }

    void OnUpdate() override{

        Hazel::RenderCommand::SetClearColor({0.45f, 0.55f, 0.60f, 1.00f});
        Hazel::RenderCommand::Clear();

        m_Camera.SetPosition({0.5f, 0.5f, 0.0f});
        m_Camera.SetRotation(45.0f);

        Hazel::Renderer::BeginScene(m_Camera);
        Hazel::Renderer::Submit(m_BlueShader,m_SquareVA);
        Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        Hazel::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {

    }

    void OnEvent(Hazel::Event& event) override {

    }
private:
    std::shared_ptr<Hazel::Shader> m_Shader;
    std::shared_ptr<Hazel::VertexArray> m_VertexArray;
    std::shared_ptr<Hazel::Shader> m_BlueShader;
    std::shared_ptr<Hazel::VertexArray> m_SquareVA;
    Hazel::OrthographicCamera m_Camera;
};

class Sandbox : public Hazel::Application {
public:
    Sandbox(){
        PushOverlay(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication() {
    return new Sandbox();
}