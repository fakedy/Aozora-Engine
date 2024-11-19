#include "Model.h"
#include "ResourceManager/ResourceManager.h"

namespace Aozora {

    // dependent on opengl which is bad but I'm not sure how to design it

    Model::Model()
    {
        ResourceManager* source = new ResourceManager(); // singleton
        m_meshes = source->loadModel("Resources/Zhu-yuan/source/Zhu-yuan.fbx"); // temp default object
        //m_meshes = source->loadModel("Resources/hintze-hall-vr-tour/source/hintze-hall_UV_pack01.fbx"); // temp default object
        //m_meshes = source->loadModel("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
        //m_meshes = source->loadModel("Resources/sponza/sponza.obj");


    }
    void Model::draw()
    {

        for (Mesh& mesh : m_meshes) {
            mesh.draw();
        }
    }
}
