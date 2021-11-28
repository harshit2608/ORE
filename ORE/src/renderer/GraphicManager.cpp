#include "GraphicManager.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include "FrameBufer.h"

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

    Ref<ManagerTexture2D> ManagerTexture2D::Create(uint32_t width, uint32_t height)
    {
        return CreateRef<Texture>(width, height);
    }

    Ref<ManagerTexture2D> ManagerTexture2D::Create(const std::string &path)
    {
        return CreateRef<Texture>(path);
    }

    // Ref<ManagerFrameBuffer> ManagerFrameBuffer::Create(const FrameBufferSpecification &spec)
    // {
    //     return CreateRef<FrameBuffer>(spec);
    // }
} // namespace ORE
