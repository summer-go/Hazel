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

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
    };

    struct Renderer2DData{
        const uint32_t MaxQuads = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
    };

    static Renderer2DData* s_Data;

    void Renderer2D::Init() {
        HZ_PROFILE_FUNCTION();

        s_Data = new Renderer2DData();
        s_Data->QuadVertexArray = Hazel::VertexArray::Create();
        // 创建顶点缓冲，按照预设的最大值MaxVertices来申请空间
        s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));
        // 设置顶点数据的布局属性，按照position、color、texCoord的顺序排列
        s_Data->QuadVertexBuffer->SetLayout(
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float4, "a_Color"},
                        {ShaderDataType::Float2, "a_TexCoord"}
                }
                );
        // 顶点缓冲绑定到顶点数组中，s_Data->QuadVertexBuffer在GPU内存中，现在只有内存占用无数据
        s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);
        // 创建CPU空间的顶点数据，也是按照最大预设置来创建
        s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];
        // 创建索引数组
        uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
        uint32_t  offset = 0;
        // 1个矩形对应4个顶点，对应6个索引值，所以offset间隔为4,indice间隔为6
        for (uint32_t i = 0; i < s_Data->MaxIndices; i+= 6) {
            quadIndices[i+0] = offset + 0;
            quadIndices[i+1] = offset + 1;
            quadIndices[i+2] = offset + 2;

            quadIndices[i+3] = offset + 2;
            quadIndices[i+4] = offset + 3;
            quadIndices[i+5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
        // 绑定索引缓冲到顶点数组
        s_Data->QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        // 创建1*1的纯色纹理
        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        // 纹理颜色为白色
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data->TextureShader = Shader::Create("../assets/shaders/Texture.glsl");
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

        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
    }

    void Renderer2D::EndScene() {
        HZ_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;
        s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    // 每个矩形对应4个顶点，即每绘制一个矩形，要添加4个顶点到s_Data中，用s_Data->QuadVertexBufferPtr标记end的地址
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
        HZ_PROFILE_FUNCTION();

        s_Data->QuadVertexBufferPtr->Position = position;
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = {position.x + size.x, position.y, 0.0f};
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = {position.x+size.x, position.y+size.y, 0.0f};
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = {position.x, position.y + size.y, 0.0f};
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadIndexCount += 6;
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