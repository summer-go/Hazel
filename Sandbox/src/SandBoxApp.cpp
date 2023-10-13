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
#include "glm/gtc/type_ptr.hpp"


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
        Hazel::Ref<Hazel::VertexBuffer> vertexBuffer(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
                {Hazel::ShaderDataType::Float3, "a_Position"},
                {Hazel::ShaderDataType::Float4, "a_Color"},
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        Hazel::Ref<Hazel::IndexBuffer> indexBuffer(Hazel::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

        m_VertexArray->SetIndexBuffer(indexBuffer);

        // -----------------矩形--------------------
        m_SquareVA.reset(Hazel::VertexArray::Create());
        // 注意矩形，顶点按照逆时针排列
        float squareVertices[5*4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        Hazel::Ref<Hazel::VertexBuffer> squareVB(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
                                    {Hazel::ShaderDataType::Float3, "a_Position"},
                                    {Hazel::ShaderDataType::Float2, "a_TexCoord"}
                            });
        m_SquareVA->AddVertexBuffer(squareVB);
        uint32_t squareIndices[6] = {0,1,2,2,3,0};
        Hazel::Ref<Hazel::IndexBuffer> squareIB;
        squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices)/sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);


        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

        std::string flatColorShaderVertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main() {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string flatColorShaderFragmentSrc = R"(
            #version 330 core
            layout(location=0) out vec4 color;
            in vec3 v_Position;
            uniform vec3 u_Color;

            void main() {
                color = vec4(u_Color, 1.0);
            }
        )";

        m_FlatColorShader = Hazel::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
        auto textureShader = m_ShaderLibrary.Load("../assets/shaders/Texture.glsl");

        m_Texture = Hazel::Texture2D::Create("../assets/textures/Checkerboard.png");
        m_ChernoLogo = Hazel::Texture2D::Create("../assets/textures/ChernoLogo.png");
        textureShader->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Hazel::Timestep ts) override{

        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts ;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts ;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts ;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        Hazel::RenderCommand::SetClearColor({0.45f, 0.55f, 0.60f, 1.00f});
        Hazel::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Hazel::Renderer::BeginScene(m_Camera);

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transfrom = glm::translate(glm::mat4(1.0f), pos) * scale;
                Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transfrom);
            }
        }

        auto textureShader = m_ShaderLibrary.Get("Texture");

        m_Texture->Bind();
        Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        m_ChernoLogo->Bind();
        Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        // Triangle
        // Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        Hazel::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnEvent(Hazel::Event& event) override {

    }

private:
    Hazel::ShaderLibrary m_ShaderLibrary;
    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;
    Hazel::Ref<Hazel::Shader> m_FlatColorShader;
    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogo;

    Hazel::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed = 5.f;
    float m_CameraRotationSpeed = 180.0f;
    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
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