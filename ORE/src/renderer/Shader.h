#pragma once

#include "../core/Log.h"
#include "../core/Core.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ORE
{
    class Shader
    {
    public:
        Shader(const std::string &filepath);
        Shader(const std::string &vertexSource, const std::string &fragmentSource);

        ~Shader();

        void Bind() const;
        void UnBind() const;

        const uint32_t GetRendererID() const { return m_RendererID; }
        const std::string &GetName() const { return m_Name; }

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;

        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w);
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        std::string ReadShaderFile(const std::string &filepath);
        std::unordered_map<uint32_t, std::string> Process(const std::string &source);
        void Compile(std::unordered_map<uint32_t, std::string> &shaderSources);

    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
} // namespace ORE
