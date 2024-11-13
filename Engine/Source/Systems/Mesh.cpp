#include "Mesh.h"
#include "ResourceManager/ResourceManager.h"
#include "glad/glad.h"
#include <iostream>

namespace Aozora {

    // dependent on opengl which is bad but I'm not sure how to design it

    Mesh::Mesh()
    {
        ResourceManager* source = new ResourceManager();
        std::unique_ptr<ModelLoader::MeshData> data = source->loadModel("Resources/cube/cube.obj"); // temp default object

        indicesSize = data->indices.size();


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(ModelLoader::Vertex), &data->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(unsigned int),
            &data->indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (void*)offsetof(ModelLoader::Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (void*)offsetof(ModelLoader::Vertex, TexCoords));

        glBindVertexArray(0);


    }
}
