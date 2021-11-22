#include "VertexArray.h"

namespace ORE
{
    static GLenum ShaderDataTypeToBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Vec:
            return GL_FLOAT;
        case ShaderDataType::Vec2:
            return GL_FLOAT;
        case ShaderDataType::Vec3:
            return GL_FLOAT;
        case ShaderDataType::Vec4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        }

        ORE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::UnBind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const Ref<ManagerVertexBuffer> &vertexBuffer)
    {
        ORE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        const auto &layout = vertexBuffer->GetLayout();
        for (const auto &element : layout)
        {
            switch (element.Type)
            {
            case ShaderDataType::Vec:
            case ShaderDataType::Vec2:
            case ShaderDataType::Vec3:
            case ShaderDataType::Vec4:
            {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribPointer(m_VertexBufferIndex,
                                      element.GetComponentCount(),
                                      ShaderDataTypeToBaseType(element.Type),
                                      element.Normalized ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      (const void *)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
            {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribIPointer(m_VertexBufferIndex,
                                       element.GetComponentCount(),
                                       ShaderDataTypeToBaseType(element.Type),
                                       layout.GetStride(),
                                       (const void *)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                          count,
                                          ShaderDataTypeToBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void *)(element.Offset + sizeof(float) * count * i));
                    glVertexAttribDivisor(m_VertexBufferIndex, 1);
                    m_VertexBufferIndex++;
                }
                break;
            }
            default:
                ORE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const Ref<ManagerIndexBuffer> &indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
} // namespace ORE
