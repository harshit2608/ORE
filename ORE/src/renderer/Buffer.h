#pragma once

#include "../core/Core.h"
#include "../core/Log.h"
#include "GraphicManager.h"

#include <glad/glad.h>

namespace ORE
{
    class VertexBuffer : public ManagerVertexBuffer
    {
    public:
        VertexBuffer(uint32_t size);
        VertexBuffer(float *vertices, uint32_t size);
        virtual ~VertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetData(const void *data, uint32_t size) override;

        virtual const BufferLayout &GetLayout() const { return m_Layout; }
        virtual void SetLayout(const BufferLayout &layout) { m_Layout = layout; }

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class IndexBuffer : public ManagerIndexBuffer
    {
    public:
        IndexBuffer(uint32_t *indices, uint32_t count);
        virtual ~IndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
} // namespace ORE
