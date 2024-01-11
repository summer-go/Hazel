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
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DData{
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        // 纹理最大值取决于GPU，至少有8个，我的mac笔记本上是16个
        static const uint32_t MaxTexturesSlots = 16; // TODO:RenderCaps

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTexturesSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        glm::vec4 QuadVertexPositions[4]; // 矩形的四个顶点

        Renderer2D::Statistics Stats;
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
                        {ShaderDataType::Float2, "a_TexCoord"},
                        {ShaderDataType::Float, "a_TexIndex"},
                        {ShaderDataType::Float, "a_TilingFactor"},
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

        int32_t samplers[Hazel::Renderer2DData::MaxTexturesSlots];
        for (uint32_t i = 0; i < Hazel::Renderer2DData::MaxTexturesSlots; i++) {
            samplers[i] = i;
        }

        s_Data->TextureShader = Shader::Create("../assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetIntArray("u_Textures", samplers, Hazel::Renderer2DData::MaxTexturesSlots);

        // Set all texture slots to 0
        // 安全起见，将数组中的每个值都初始化成一个默认的纹理，即单像素纹理
        for (int i = 0; i < s_Data->MaxTexturesSlots; i++) {
            s_Data->TextureSlots[i] = s_Data->WhiteTexture;
        }

        // 初始化单位矩阵的顶点坐标
        s_Data->QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        s_Data->QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        s_Data->QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        s_Data->QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    }

    void Renderer2D::Shutdown() {
        HZ_PROFILE_FUNCTION();

        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
        s_Data->TextureSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) {
        HZ_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

        s_Data->TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene() {
        HZ_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;
        s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        // Bind Textures

        for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++) {
            s_Data->TextureSlots[i]->Bind(i);
        }

        // 这里绑定TextureSlotIndex个纹理，但是前面声明了16个纹理，并且在shader中也声明了16个纹理，运行时，会按索引查找所有的纹理，mac上会抛出如下警告日志
        // 如果你想消除这个警告，将所有的索引都绑定到gpu的纹理上，即将上面这段代码替换成下面这段。

        //UNSUPPORTED (log once): POSSIBLE ISSUE: unit 2 GLD_TEXTURE_INDEX_2D is unloadable and bound to sampler type (Float) - using zero texture because texture unloadable
        for (uint32_t i = 0; i < s_Data->MaxTexturesSlots; i++) {
            s_Data->TextureSlots[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
        s_Data->Stats.DrawCalls++;
    }

    void Renderer2D::FlushAndReset() {
        EndScene();

        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

        s_Data->TextureSlotIndex = 1;
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    // 每个矩形对应4个顶点，即每绘制一个矩形，要添加4个顶点到s_Data中，用s_Data->QuadVertexBufferPtr标记end的地址
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
        HZ_PROFILE_FUNCTION();

        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices) {
            FlushAndReset();
        }

        const float texIndex = 0.0f; // white Texture
        const float tilingFactor = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadIndexCount += 6;

        s_Data->Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4& tintColor) {
        DrawQuad({position.x, position.y, 0.f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4& tintColor) {
        HZ_PROFILE_FUNCTION();

        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices) {
            FlushAndReset();
        }

        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

        float texIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->TextureSlotIndex; ++i) {
            if (*s_Data->TextureSlots[i].get() == *texture.get()) {
                texIndex = (float)i;
                break;
            }
        }

        if (0.0f == texIndex) {
            texIndex = (float)s_Data->TextureSlotIndex;
            s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
            s_Data->TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadIndexCount += 6;

        s_Data->Stats.QuadCount++;

#if OLD_PATH
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
        s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0), {size.x, size.y, 1.0f});
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
#endif
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color) {
        DrawQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const glm::vec4 &color) {
        HZ_PROFILE_FUNCTION();

        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices) {
            FlushAndReset();
        }

        const float texIndex = 0.0f; // white Texture
        const float tilingFactor = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});


        s_Data->TextureShader->SetFloat4("u_Color", color);
        s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->Bind();


        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadIndexCount += 6;

        s_Data->Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                     const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
        DrawQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                     const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
        HZ_PROFILE_FUNCTION();

        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices) {
            FlushAndReset();
        }

        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

        float texIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->TextureSlotIndex; ++i) {
            if (*s_Data->TextureSlots[i].get() == *texture.get()) {
                texIndex = (float)i;
                break;
            }
        }

        if (0.0f == texIndex) {
            texIndex = (float)s_Data->TextureSlotIndex;
            s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
            s_Data->TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
                              * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
        s_Data->QuadVertexBufferPtr->Color = color;
        s_Data->QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
        s_Data->QuadVertexBufferPtr->TexIndex = texIndex;
        s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data->QuadVertexBufferPtr++;

        s_Data->QuadIndexCount += 6;

        s_Data->Stats.QuadCount++;
    }

    void Renderer2D::ResetStats() {
        memset(&s_Data->Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_Data->Stats;
    }

}