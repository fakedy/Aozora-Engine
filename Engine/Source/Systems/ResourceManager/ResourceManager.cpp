#include "ResourceManager.h"
#include <iostream>

namespace Aozora {

    ResourceManager* ResourceManager::m_resourceManager = nullptr;

    ResourceManager::ResourceManager()
    {
        if (m_resourceManager == nullptr) {
            m_resourceManager = this;
        }
        m_modelLoader = new ModelLoader();
    }

    const std::vector<Mesh> ResourceManager::loadModel(const std::string& file)
    {

        // I think a system that will check if the model is already loaded is good here.
        // it will ensure we dont load multiple of the same models into memory

        // TODO store model into memory

        return m_modelLoader->loadModel(file);

    }
}


