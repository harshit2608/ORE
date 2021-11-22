#include "GraphicManager.h"

#include "VertexArray.h"
#include "Buffer.h"

namespace ORE
{
    Ref<ManagerVertexBuffer> ManagerVertexBuffer::Create(uint32_t size)
    {
        return CreateRef<VertexBuffer>(size);
    }

    Ref<ManagerVertexBuffer> ManagerVertexBuffer::Create(float *vertices, uint32_t size)
    {
        return CreateRef<VertexBuffer>(vertices, size);
    }

    Ref<ManagerIndexBuffer> ManagerIndexBuffer::Create(uint32_t *indices, uint32_t size)
    {
        return CreateRef<IndexBuffer>(indices, size);
    }
    Ref<ManagerVertexArray> ManagerVertexArray::Create()
    {
        return CreateRef<VertexArray>();
    }
} // namespace ORE
