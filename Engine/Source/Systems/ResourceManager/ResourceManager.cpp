#include "ResourceManager.h"
#include <Systems/Logging/Logger.h>
#include <variant>
#include <random>

namespace Aozora {


    const void ResourceManager::loadModel(uint64_t hash, uint64_t sceneID)
    {
       
       // if it is not cached
       if (!modelLoaded(hash, sceneID)) {
           const Model& model = m_assetManager.loadModelFromDisk(hash);

           for (const auto& node : model.allNodes) {
               if (node.hasMesh) {

                   
                   uint64_t meshID = loadMesh(node.meshID, sceneID);
                   uint64_t matID = m_containerMap[sceneID].m_loadedMeshes[meshID].materialID;
                   loadMaterial(matID, sceneID);

                   for (uint64_t texID : m_containerMap[sceneID].m_loadedmaterials[matID].textureIDs) {
                       loadTexture(texID, sceneID);
                   }
               }
           }
           m_containerMap[sceneID].m_loadedModels.emplace(hash, model);

       }
       else {
           Log::info(std::format("Using cache for model: {}", hash));
       }
    }

    // opengl dependent code for loading texture
    uint64_t ResourceManager::loadTexture(uint64_t hash, uint64_t sceneID)
    {
        
        unsigned int texture = 0;
        if (textureLoaded(hash, sceneID)) {

            return hash;
        }

        Texture tex = m_assetManager.loadTextureFromDisk(hash);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        GLenum internalFormat;
        GLenum sourceFormat;
        GLenum sourceType;

        std::visit([&](auto&& data) {

            using VecType = std::decay_t<decltype(data[0])>;
            using PixelType = typename VecType::value_type;

            // if the image is HDR we assume its also in linear space
            if (std::is_same_v<PixelType, float>) {
                internalFormat = GL_R11F_G11F_B10F;
                sourceFormat = GL_RGB;
                sourceType = GL_FLOAT;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, sourceFormat, sourceType, data[0].data());
                data[0].clear();
                data[0].shrink_to_fit();
            }
            else {
                if (tex.isSrgb) { // if the image is in gamma mode
                    internalFormat = (tex.nrChannels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
                }
                else {
                    internalFormat = (tex.nrChannels == 4) ? GL_RGBA8 : GL_RGB8;
                }
                sourceFormat = (tex.nrChannels == 4) ? GL_RGBA : GL_RGB;
                sourceType = GL_UNSIGNED_BYTE;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, sourceFormat, sourceType, data[0].data());
                data[0].clear();
                data[0].shrink_to_fit();
            }
        }, tex.dataVector);

        
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint64_t handle = glGetTextureHandleARB(texture);
        glMakeTextureHandleResidentARB(handle);
        tex.handle = handle;
        m_containerMap[sceneID].m_loadedTextures[hash] = tex;
        Log::info(std::format("Created texture with ID: {}", texture));

        return texture;
        
    }

    uint64_t ResourceManager::loadTexturePersistent(uint64_t hash)
    {
        unsigned int texture = 0;
        /*
        if (textureLoaded(hash, sceneID)) {

            return hash;
        }
        */
        Texture tex = m_assetManager.loadTextureFromDisk(hash);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        GLenum internalFormat;
        GLenum sourceFormat;
        GLenum sourceType;

        std::visit([&](auto&& data) {

            using VecType = std::decay_t<decltype(data[0])>;
            using PixelType = typename VecType::value_type;

            // if the image is HDR we assume its also in linear space
            if (std::is_same_v<PixelType, float>) {
                internalFormat = GL_R11F_G11F_B10F;
                sourceFormat = GL_RGB;
                sourceType = GL_FLOAT;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, sourceFormat, sourceType, data[0].data());
                data[0].clear();
                data[0].shrink_to_fit();
            }
            else {
                if (tex.isSrgb) { // if the image is in gamma mode
                    internalFormat = (tex.nrChannels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
                }
                else {
                    internalFormat = (tex.nrChannels == 4) ? GL_RGBA8 : GL_RGB8;
                }
                sourceFormat = (tex.nrChannels == 4) ? GL_RGBA : GL_RGB;
                sourceType = GL_UNSIGNED_BYTE;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, sourceFormat, sourceType, data[0].data());
                data[0].clear();
                data[0].shrink_to_fit();
            }
            }, tex.dataVector);


        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint64_t handle = glGetTextureHandleARB(texture);
        glMakeTextureHandleResidentARB(handle);
        tex.handle = handle;
        m_loadedPersistentTextures[hash] = tex;
        Log::info(std::format("Created texture with ID: {}", texture));

        return texture;
    }

    uint64_t ResourceManager::loadMaterial(uint64_t hash, uint64_t sceneID)
    {
        if (hash != 0) {
            if (!materialLoaded(hash, sceneID)) {
                m_containerMap[sceneID].m_loadedmaterials[hash] = m_assetManager.loadMaterialFromDisk(hash);
            }
        }
        return hash;
    }

    uint64_t ResourceManager::loadCubemap(uint64_t hash, uint64_t sceneID)
    {

        Texture texture = m_assetManager.loadTextureFromDisk(hash);

        uint32_t textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        GLenum internalFormat;
        GLenum sourceFormat;
        GLenum sourceType;

        std::visit([&](auto&& data) {

            using VecType = std::decay_t<decltype(data[0])>;
            using PixelType = typename VecType::value_type;
            int width = texture.width;
            int height = texture.height;
            // if the image is HDR we assume its also in linear space
            if (std::is_same_v<PixelType, float>) {
                internalFormat = GL_R11F_G11F_B10F;
                sourceFormat = GL_RGB;
                sourceType = GL_FLOAT;
            }
            else {
                internalFormat = (texture.nrChannels == 4) ? GL_RGBA8 : GL_RGB8;
                sourceFormat = (texture.nrChannels == 4) ? GL_RGBA : GL_RGB;
                sourceType = GL_UNSIGNED_BYTE;
         
            }

            for (int i = 0; i < 6; i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, sourceFormat, sourceType, data[i].data());
                data[i].clear();
                data[i].shrink_to_fit();
            }
        }, texture.dataVector);


        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        texture.gpuID = textureID;

        m_containerMap[sceneID].m_loadedTextures[texture.id] = texture;
        return textureID;
    }

    uint64_t ResourceManager::createEmptyCubeMap(uint32_t width, uint32_t height, uint64_t sceneID)
    {

        Texture tex;
        uint32_t textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        GLenum internalFormat;
        GLenum sourceFormat;
        GLenum sourceType;



        internalFormat = GL_R11F_G11F_B10F;
        sourceFormat = GL_RGBA;
        sourceType = GL_FLOAT;
        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, sourceFormat, sourceType, nullptr);
        }


        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        tex.gpuID = textureID;

        // to get a "unique" id, however we dont check wether the id is unique, this can and will cause problems
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;
        tex.id = dis(gen);
        m_containerMap[sceneID].m_loadedTextures[tex.id] = tex;
        return tex.id;

    }

    uint64_t ResourceManager::loadSkybox(uint64_t hash, uint64_t sceneID)
    {
            // yes its confusing, yes its temporary, said the guy who now dont remember what is confusing about this
            // must be because of how messy this is to read
            Skybox skybox = m_assetManager.loadSkyboxFromDisk(hash);
            loadCubemap(skybox.cubeMapTexture, sceneID);

            skybox.irradienceMapTexture = createEmptyCubeMap(32, 32, sceneID);
            m_renderAPI.bakeCubemapIrradiance(m_containerMap[sceneID].m_loadedTextures[skybox.cubeMapTexture].gpuID,
            m_containerMap[sceneID].m_loadedTextures[skybox.irradienceMapTexture].gpuID);

            m_containerMap[sceneID].m_loadedSkyboxes[skybox.id] = skybox;
            return skybox.id;
    }

    uint64_t ResourceManager::loadMesh(uint64_t hash, uint64_t sceneID)
    {
        if (!meshLoaded(hash, sceneID)) {
            m_containerMap[sceneID].m_loadedMeshes[hash] = m_assetManager.loadMeshFromDisk(hash);
        }
        return hash; 
    }

    uint64_t ResourceManager::createMaterial(Material* material)
    {
        return 0;
    }

    // check if material is loaded, if it is loaded return it's id, else return -1
    uint64_t ResourceManager::materialLoaded(uint64_t id, uint64_t sceneID)
    {

        auto it = m_containerMap[sceneID].m_loadedmaterials.find(id);
        if (it != m_containerMap[sceneID].m_loadedmaterials.end()) {
            Log::info("Material in RAM, using cached version");
            return true;
        }

        return false;
    }

    Material& ResourceManager::getMaterial(uint64_t id, uint64_t sceneID)
    {
        return m_containerMap[sceneID].m_loadedmaterials[id];
    }
    

    // Check if texture is loaded, if it is loaded return it's id, else return 0
    uint64_t ResourceManager::textureLoaded(uint64_t hash, uint64_t sceneID)
    {
        auto it = m_containerMap[sceneID].m_loadedTextures.find(hash);
        if (it != m_containerMap[sceneID].m_loadedTextures.end()){
            Log::info("Texture in RAM, using cached version");
            return true;
        }
        return false;;
    }

    // return the index of the loaded mesh or 0 if its not loaded
    uint64_t ResourceManager::meshLoaded(uint64_t hash, uint64_t sceneID)
    {
        auto it = m_containerMap[sceneID].m_loadedMeshes.find(hash);
        if (it != m_containerMap[sceneID].m_loadedMeshes.end()) {
            Log::info("Mesh in RAM, using cached version");
            return true;
        }

        return false;
    }

    uint64_t ResourceManager::modelLoaded(uint64_t hash, uint64_t sceneID) {

        auto it = m_containerMap[sceneID].m_loadedModels.find(hash);
        if (it != m_containerMap[sceneID].m_loadedModels.end()) {
            return true;
        }
        return false;
    }

    void ResourceManager::clearResources(uint64_t sceneID)
	{
        Log::info("Clearing resources");


        // list of textures to delete
        std::vector<GLuint> texturesToDelete;

        // because we cant get a clean internal datastructure of an unordered map
        for (auto& texture : m_containerMap[sceneID].m_loadedTextures) {
            texturesToDelete.push_back(texture.second.id);
        }
        
        if (!texturesToDelete.empty()) {
            // by using the vector we save gl calls.
            glDeleteTextures(texturesToDelete.size(), texturesToDelete.data());
        }
        // finally clear the old cpu data
        m_containerMap[sceneID].m_loadedTextures.clear();


        for (auto& mesh : m_containerMap[sceneID].m_loadedMeshes) {
            glDeleteBuffers(1, &mesh.second.VBO);
            glDeleteBuffers(1, &mesh.second.EBO);
            glDeleteVertexArrays(1, &mesh.second.VAO);
        }
        m_containerMap[sceneID].m_loadedMeshes.clear();

        m_containerMap[sceneID].m_loadedmaterials.clear();
        m_containerMap[sceneID].m_loadedModels.clear();
        m_containerMap[sceneID].m_nextMeshID = 0;
        m_containerMap[sceneID].m_nextMaterialID = 0;
    }

}


