#pragma once

#include "../core/Core.h"
#include "../core/Log.h"
#include "../renderer/Shader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_BONE_INFLUENCE 4

namespace ORE
{
    struct AssetVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        uint32_t m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct AssetTexture
    {
        uint32_t id;
        std::string type;
        std::string path;
    };

    struct Transformations
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        float initial_scale;
    };

    class Mesh
    {
    public:
        Mesh(std::vector<AssetVertex> vertices, std::vector<unsigned int> indices, std::vector<AssetTexture> textures);
        void Draw(Shader &shader);

    private:
        void setupMesh();

    private:
        std::vector<AssetVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<AssetTexture> textures;
        uint32_t VAO, VBO, EBO;
    };
} // namespace ORE
