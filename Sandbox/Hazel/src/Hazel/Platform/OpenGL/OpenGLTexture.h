#pragma once

#include <string>
#include "Renderer/Texture.h"
namespace Hazel {
    class OpenGLTexture2D : public Texture2D{
    public:
        explicit OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        [[nodiscard]] uint32_t GetWidth() const override { return m_width;}
        [[nodiscard]] uint32_t GetHeight() const override { return m_height;}

        void Bind(uint32_t slot = 0) const override;

    private:
        std::string m_Path;
        uint32_t m_width, m_height;
        uint32_t m_RendererID;
    };
}
