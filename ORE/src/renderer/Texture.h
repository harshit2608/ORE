#pragma once

#include "../core/Core.h"
#include "GraphicManager.h"

#include <glad/glad.h>

namespace ORE
{
    class Texture : public ManagerTexture2D
    {
    public:
        Texture(uint32_t width, uint32_t height);
        Texture(const std::string &path);
        virtual ~Texture();

        virtual void Bind(uint32_t slot = 0) const override;

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetRendererID() const override { return m_RendererID; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Width, m_Height;
        std::string m_Path;
        GLenum m_InternalFormat, m_DataFormat;
    };
} // namespace ORE
