#pragma once

#include "../core/Core.h"
#include "../core/Log.h"

#include <glad/glad.h>

namespace ORE
{
    enum class ShaderDataType
    {
        None = 0,
        Vec,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Vec:
            return 4;
        case ShaderDataType::Vec2:
            return 4 * 2;
        case ShaderDataType::Vec3:
            return 4 * 3;
        case ShaderDataType::Vec4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }

        ORE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Vec:
                return 1;
            case ShaderDataType::Vec2:
                return 2;
            case ShaderDataType::Vec3:
                return 3;
            case ShaderDataType::Vec4:
                return 4;
            case ShaderDataType::Mat3:
                return 3;
            case ShaderDataType::Mat4:
                return 4;
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
            }

            ORE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return m_Stride; }
        const std::vector<BufferElement> &GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto &element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };
    class ManagerVertexBuffer
    {
    public:
        virtual ~ManagerVertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void SetData(const void *data, uint32_t size) = 0;

        virtual const BufferLayout &GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout &layout) = 0;

        static Ref<ManagerVertexBuffer> Create(uint32_t size);
        static Ref<ManagerVertexBuffer> Create(float *vertices, uint32_t size);
    };

    class ManagerIndexBuffer
    {
    public:
        virtual ~ManagerIndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<ManagerIndexBuffer> Create(uint32_t *indices, uint32_t count);
    };

    class ManagerVertexArray
    {
    public:
        virtual ~ManagerVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(const Ref<ManagerVertexBuffer> &vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<ManagerIndexBuffer> &indexBuffer) = 0;

        virtual const std::vector<Ref<ManagerVertexBuffer>> &GetVertexBuffers() const = 0;
        virtual const Ref<ManagerIndexBuffer> &GetIndexBuffer() const = 0;

        static Ref<ManagerVertexArray> Create();
    };

    class ManagerTexture
    {
    public:
        virtual ~ManagerTexture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class ManagerTexture2D : public ManagerTexture
    {
    public:
        static Ref<ManagerTexture2D> Create(uint32_t width, uint32_t height);
        static Ref<ManagerTexture2D> Create(const std::string &path);
    };

    enum class ManagerFrameBufferTextureFormat
    {
        None = 0,
        RGBA8,
        DEPTH24STENCIL8,
        Depth = DEPTH24STENCIL8
    };

    struct ManagerFrameBufferTextureSpecification
    {
        ManagerFrameBufferTextureSpecification() = default;
        ManagerFrameBufferTextureSpecification(ManagerFrameBufferTextureFormat format)
            : TextureFormat(format) {}

        ManagerFrameBufferTextureFormat TextureFormat = ManagerFrameBufferTextureFormat::None;
        // TODO: filtering/wrap
    };

    struct FrameBufferAttachmentSpecification
    {
        FrameBufferAttachmentSpecification() = default;
        FrameBufferAttachmentSpecification(std::initializer_list<ManagerFrameBufferTextureSpecification> attachments)
            : Attachments(attachments) {}

        std::vector<ManagerFrameBufferTextureSpecification> Attachments;
    };

    struct FrameBufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        FrameBufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class ManagerFrameBuffer
    {
    public:
        virtual ~ManagerFrameBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

        virtual const FrameBufferSpecification &GetSpecification() const = 0;

        static Ref<ManagerFrameBuffer> Create(const FrameBufferSpecification &spec);
    };
    
} // namespace ORE
