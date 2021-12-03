#include "Model.h"
#include "../renderer/Texture.h"

namespace ORE
{
    Model::Model(const std::string &path, float scaleFactor, bool gamma)
        : m_ScaleFactor(scaleFactor), m_gammaCorrection(gamma)
    {
        loadModel(path);
    }

    void Model::Draw(Shader &shader)
    {
        for (uint32_t i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader);
        }
    }

    void Model::loadModel(std::string const &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            ORE_CORE_ERROR("Assimp Error: {0} ", importer.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode *node, const aiScene *scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    std::vector<AssetTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<AssetTexture> textures;
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (uint32_t j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                AssetTexture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<AssetVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<AssetTexture> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            AssetVertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiString albedoPath;
        std::vector<AssetTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<AssetTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<AssetTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<AssetTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(vertices, indices, textures);
    }

    void Model::CheckForCache(const char *path, const char *binPath, ImageData &imageData)
    {
        FILE *cachedFile = fopen(binPath, "rb");
        // No cache file for texture exists. Load and save it
        if (cachedFile == nullptr)
        {
            imageData.data = stbi_load(path, &imageData.width, &imageData.height, &imageData.channels, 0);
            if (imageData.data == nullptr)
            {
                ORE_CORE_ERROR("Failed to open texture at path: {0}", path);
                exit(-1);
            }

            // Write the metadata and the actual image's data to the cache
            FILE *outFile = fopen(binPath, "wb");
            if (outFile == nullptr)
            {
                ORE_CORE_ERROR("Failed to create output binary file at path: {0}", binPath);
                exit(-1);
            }
            fprintf(outFile, "%d %d %d\n", imageData.width, imageData.height, imageData.channels);
            fwrite(imageData.data, sizeof(unsigned char) * (size_t)imageData.width * imageData.height * imageData.channels, 1, outFile);

            // Cleanup
            fclose(outFile);
            ORE_CORE_INFO("Created cache for image at path: {0}", path);
        }

        // Cache file exists, load that instead
        else
        {
            // Get image metadata
            if (fscanf(cachedFile, "%d %d %d\n", &imageData.width, &imageData.height, &imageData.channels) == EOF)
            {
                ORE_CORE_ERROR("Invalid image metadata contained in file: {0}", binPath);
            }

            // Get the image's actual data
            size_t dataSize = (size_t)imageData.width * imageData.height * imageData.channels;
            imageData.data = new unsigned char[dataSize + 1];
            fread(imageData.data, sizeof(unsigned char) * dataSize, 1, cachedFile);
            imageData.data[dataSize] = '\0';

            // Cleanup
            fclose(cachedFile);
            ORE_CORE_INFO("Loaded cache at path: {0}", binPath);
        }
    }

    uint32_t Model::TextureFromFile(const char *path, const std::string &directory, bool gamma)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        std::string binPath(filename);
        binPath = binPath.substr(0, binPath.find_last_of('.')) + ".bin";
        ImageData imageData = {};

        // CheckForCache(filename.c_str(), binPath.c_str(), imageData);

        // Texture *m_texture = new Texture(filename);
        // m_texture = ManagerTexture2D::Create(filename);
        // m_texture->Bind();
        // return m_texture->GetRendererID();
        uint32_t textureID;
        glGenTextures(1, &textureID);

        // int width, height, channels;
        imageData.data = stbi_load(filename.c_str(), &imageData.width, &imageData.height, &imageData.channels, 0);
        if (imageData.data)
        {
            GLenum format, internalFormat;
            if (imageData.channels == 1)
            {
                format = GL_RED;
                internalFormat = GL_R8;
            }
            else if (imageData.channels == 2)
            {
                format = GL_RG;
                internalFormat = GL_RG8;
            }
            else if (imageData.channels == 3)
            {
                format = GL_RGB;
                internalFormat = GL_RGB8;
            }
            else if (imageData.channels == 4)
            {
                format = GL_RGBA;
                internalFormat = GL_RGBA8;
            }
            else
            {
                ORE_CORE_ERROR("Image loaded isn't in any of the supported formats! (Supported: RGB, RGBA)");
                stbi_image_free(imageData.data);

                glDeleteTextures(1, &textureID);
                exit(-1);
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageData.width, imageData.height, 0, format, GL_UNSIGNED_BYTE, imageData.data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(imageData.data);
            // delete[] imageData.data;
        }
        else
        {
            ORE_CORE_ERROR("Texture failed to load at path: {0}", path);
            stbi_image_free(imageData.data);
        }

        return textureID;
    }
} // namespace ORE
