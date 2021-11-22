#pragma once

#include "../core/Core.h"
#include "../core/Log.h"
#include "GraphicManager.h"

#include <glad/glad.h>

namespace ORE
{
    class VertexArray : public ManagerVertexArray
    {
    public:
        VertexArray();
        virtual ~VertexArray();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const Ref<ManagerVertexBuffer> &vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<ManagerIndexBuffer> &indexBuffer) override;

        virtual const std::vector<Ref<ManagerVertexBuffer>> &GetVertexBuffers() const { return m_VertexBuffers; }
        virtual const Ref<ManagerIndexBuffer> &GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        std::vector<Ref<ManagerVertexBuffer>> m_VertexBuffers;
        Ref<ManagerIndexBuffer> m_IndexBuffer;
    };
} // namespace ORE
