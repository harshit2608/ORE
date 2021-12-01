#pragma once

#include "../core/Core.h"
#include "../core/Log.h"
#include "../renderer/Shader.h"
#include "../renderer/GraphicManager.h"

#include "Mesh.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ORE
{
    struct ImageData
    {
        unsigned char *data;
        int width, height, channels;
    };

    class Model
    {
    public:
        Model(const std::string &path, float scaleFactor, bool gamma = false);
        void Draw(Shader &shader);
        float GetScaleFactor() const { return m_ScaleFactor; }

    private:
        std::vector<AssetTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

        void loadModel(std::string const &path);
        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        uint32_t TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
        void CheckForCache(const char *path, const char *binPath, ImageData &imageData);

    private:
        std::vector<AssetTexture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool m_gammaCorrection;
        float m_ScaleFactor;
        // Ref<ManagerTexture2D> m_texture;
    };
} // namespace ORE