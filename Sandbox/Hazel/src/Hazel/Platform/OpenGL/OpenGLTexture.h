#pragma once

#include <string>
#include "Renderer/Texture.h"
#include "OpenGLShader.h"
#include "glad/glad.h"

namespace Hazel {
    class OpenGLTexture2D : public Texture2D{
    public:
        explicit OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(uint32_t width, uint32_t height);
        ~OpenGLTexture2D() override;

        uint32_t GetWidth() const override { return m_width;}
        uint32_t GetHeight() const override { return m_height;}

        void Bind(uint32_t slot = 0) const override;

        void SetData(void *data, uint32_t size) override;


        bool operator==(const Texture &other) const override{
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }

    private:
        std::string m_Path;
        uint32_t m_width, m_height;
        uint32_t m_RendererID;

        GLint m_InternalFormat = 0;
        GLenum m_DataFormat = 0;
    };
}
