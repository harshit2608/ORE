#include "Shader.h"

namespace ORE
{
    static uint32_t shaderTypeFromString(const std::string &shader)
    {
        if (shader == "VERTEX")
        {
            return GL_VERTEX_SHADER;
        }
        if (shader == "FRAGMENT" || shader == "PIXEL")
        {
            return GL_FRAGMENT_SHADER;
        }
        return 0;
    }

    Shader::Shader(const std::string &filepath)
    {
        std::string source = ReadShaderFile(filepath);
        auto shaderSource = Process(source);
        Compile(shaderSource);

        auto slash = filepath.find_last_of("/\\");
        slash = slash == std::string::npos ? 0 : slash + 1;
        auto dot = filepath.rfind('.');
        auto count = dot == std::string::npos ? filepath.size() - slash : dot - slash;
        m_Name = filepath.substr(slash, count);
    }

    Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
    {
        std::unordered_map<uint32_t, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSource;
        sources[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(sources);
    }

    std::string Shader::ReadShaderFile(const std::string &filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                ORE_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        }
        else
        {
            ORE_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<uint32_t, std::string> Shader::Process(const std::string &source)
    {
        std::unordered_map<uint32_t, std::string> shaderSources;

        const char *typeToken = "#SHADER_TYPE";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
            ORE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#shader " keyword)
            std::string shader = source.substr(begin, eol - begin);
            ORE_CORE_ASSERT(shaderTypeFromString(shader), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
            ORE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

            shaderSources[shaderTypeFromString(shader)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void Shader::Compile(std::unordered_map<uint32_t, std::string> &shaderSources)
    {
        uint32_t program = glCreateProgram();
        ORE_CORE_ASSERT(shaderSources.size() <= 3, "We only support 3 shaders for now");
        std::array<uint32_t, 3> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto &ss : shaderSources)
        {
            uint32_t type = ss.first;
            const std::string &source = ss.second;

            uint32_t shader = glCreateShader(type);

            const char *sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            int isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                int maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<char> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                ORE_CORE_ERROR("{0}", infoLog.data());
                ORE_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        m_RendererID = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        int isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            int maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for (auto id : glShaderIDs)
                glDeleteShader(id);

            ORE_CORE_ERROR("{0}", infoLog.data());
            ORE_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::UnBind() const
    {
        glUseProgram(0);
    }

} // namespace ORE
