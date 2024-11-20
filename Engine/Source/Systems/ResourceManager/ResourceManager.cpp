#include "ResourceManager.h"


namespace Aozora {


    const std::vector<Mesh> ResourceManager::loadModel(const std::string& file)
    {

        // I think a system that will check if the model is already loaded is good here.
        // it will ensure we dont load multiple of the same models into memory

        // TODO store model into memory

        return m_modelLoader.loadModel(file);

    }

    unsigned int ResourceManager::loadTexture(const char* path)
    {

        return 0;
    }


}


