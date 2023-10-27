//
// Created by xiatian05 on 2023/10/26.
//

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {
    struct Renderer2DStorage{
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
        Ref<Shader> TextureShader;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init() {
        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = Hazel::VertexArray::Create();
        float squareVertices[5 * 4] = {
                -0.5, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5, 0.5f, 0.0f, 1.0f, 1.0f,
                -0.5, 0.5f, 0.0f, 0.0f, 1.0f
        };

        Hazel::Ref<Hazel::VertexBuffer> squareVB;
        squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
                                    {Hazel::ShaderDataType::Float3, "a_Position"},
                                    {Hazel::ShaderDataType::Float2, "a_TexCoord"},
                            });
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2,3, 0};
        Hazel::Ref<Hazel::IndexBuffer> squareIB;
        squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

        s_Data->FlatColorShader = Hazel::Shader::Create("../assets/shaders/FlatColor.glsl");
        s_Data->TextureShader = Hazel::Shader::Create("../assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);

    }

    void Renderer2D::Shutdown() {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
//        s_Data->FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0f));

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {

    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetFloat4("u_Color", color);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0), {size.x, size.y, 1.0f});
        s_Data->FlatColorShader->SetMat4("u_Transform", transform);
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture) {
        DrawQuad({position.x, position.y, 0.f}, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture) {
        s_Data->TextureShader->Bind();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0), {size.x, size.y, 1.0f});
        s_Data->FlatColorShader->SetMat4("u_Transform", transform);
        texture->Bind();

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}