#pragma once

#include "../core/Core.h"

#include <glad/glad.h>

namespace ORE
{
    class Texture
    {
    public:
        Texture(uint32_t width, uint32_t height);
        Texture(const std::string &path);
        ~Texture();

        void Bind(uint32_t slot = 0) const;

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        uint32_t GetRendererID() const { return m_RendererID; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Width, m_Height;
        std::string m_Path;
        GLenum m_InternalFormat, m_DataFormat;
    };
} // namespace ORE
