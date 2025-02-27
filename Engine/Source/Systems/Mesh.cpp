#include "Mesh.h"
#include "ResourceManager/ResourceManager.h"
#include "glad/glad.h"
#include <iostream>

namespace Aozora {

    // dependent on opengl which is bad but I'm not sure how to design it

    Mesh::Mesh()
    {
    }
    void Mesh::bufferData()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Vertex), meshData.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);

        // can clear meshdata
    }
    void Mesh::draw(Shader &shader)
    {
        unsigned int diffuseNr = 0;
        unsigned int normalNr = 0;
        unsigned int heightNr = 0;
        unsigned int emissiveNr = 0;
        unsigned int aoNr = 0;
        unsigned int metallicNr = 0;
        unsigned int roughnessNr = 0;

        glUniform3fv(glGetUniformLocation(shader.ID, "albedo"), 1, &material->baseColor[0]);
        glUniform1f(glGetUniformLocation(shader.ID, "metallic"), material->metallic);
        glUniform1f(glGetUniformLocation(shader.ID, "roughness"), material->roughness);
        glUniform1f(glGetUniformLocation(shader.ID, "ao"), material->ao);
        glUniform3fv(glGetUniformLocation(shader.ID, "emissive"), 1, &material->emissive[0]);


        for (unsigned int i = 0; i < material->activeTextures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            unsigned int num = 0;
            std::string name = material->activeTextures[i].type;
            if (name == "texture_diffuse") {
                diffuseNr++;
				num = diffuseNr;
                number = std::to_string(num);
            }
            else if (name == "texture_normal") {
                normalNr++;
                num = normalNr;
                number = std::to_string(num);
            }
            else if (name == "texture_emissive") {
                emissiveNr++;
                num = emissiveNr;
                number = std::to_string(num);
            }
            else if (name == "texture_ao") {
                aoNr++;
                num = aoNr;
                number = std::to_string(num);
            }
            else if (name == "texture_metallic") {
                metallicNr++;
                num = metallicNr;
                number = std::to_string(num);
            }
            else if (name == "texture_roughness") {
                roughnessNr++;
                num = roughnessNr;
                number = std::to_string(num);
            }

            glUniform1i(glGetUniformLocation(shader.ID, ("has_" + name + number).c_str()), num);
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, material->activeTextures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, meshData.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        for (unsigned int i = 0; i < material->activeTextures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        

    }
}
