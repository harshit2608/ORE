#pragma once

#include "../core/Log.h"
#include "../core/Core.h"

#include <glad/glad.h>

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

        uint32_t GetRendererrID() { return m_RendererID; }
        const std::string &GetName() const { return m_Name; }

    private:
        std::string ReadShaderFile(const std::string &filepath);
        std::unordered_map<uint32_t, std::string> Process(const std::string &source);
        void Compile(std::unordered_map<uint32_t, std::string> &shaderSources);

    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
} // namespace ORE
