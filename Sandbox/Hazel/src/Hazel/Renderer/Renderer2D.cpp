//
// Created by xiatian05 on 2023/10/26.
//

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"
#include "Debug/Instrumentor.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {
    struct Renderer2DStorage{
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init() {
        HZ_PROFILE_FUNCTION();

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

        s_Data->WhiteTexture = Texture2D::Create(1,1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

//        s_Data->TextureShader = Hazel::Shader::Create("../assets/shaders/Texture.glsl");
        s_Data->TextureShader = Hazel::Shader::Create("../assets/shaders/GameTexture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown() {
        HZ_PROFILE_FUNCTION();

        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {
        HZ_PROFILE_FUNCTION();

    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Color", color);
        s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->Bind();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0), {size.x, size.y, 1.0f});
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4& tintColor) {
        DrawQuad({position.x, position.y, 0.f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4& tintColor) {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
        s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0), {size.x, size.y, 1.0f});
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color) {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Color", color);
        s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0), rotation, {0.0f, 0.0f, 1.0f})
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
        DrawQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Color", tintColor);
        s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_Data->TextureShader->SetMat4("u_Transform", transform);
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

}