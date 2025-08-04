#include "Mesh.h"
#include "ResourceManager/ResourceManager.h"
#include "glad/glad.h"
#include <iostream>
#include <AozoraAPI/Aozora.h>

namespace Aozora {



    Mesh::Mesh()
    {
    }

    // upload the data to the gpu
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
        // tangents
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex texture coords
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);

        // can clear meshdata
    }


    // need a total rework of this system with chacing the uniformlocations
    void Mesh::draw(Shader &shader)
    {
        auto& material = ResourcesAPI::getMaterial(materialID);
        shader.setVec4fv("albedo", material.baseColor);
        shader.setFloat("metallic", material.metallic);
        shader.setFloat("roughness", material.roughness);
        shader.setFloat("ao", material.ao);
        shader.setVec4fv("emissive", material.emissive);
        // bad
        shader.setInt("has_texture_diffuse", 0);
        shader.setInt("has_texture_normal", 0);
        shader.setInt("has_texture_emissive", 0);
        shader.setInt("has_texture_ao", 0);
        shader.setInt("has_texture_metallic", 0);
        shader.setInt("has_texture_roughness", 0);


        for (unsigned int i = 0; i < material.activeTextures.size(); i++) {
            std::string name;
            int textureUnit = 0;
            Material::TextureType type = material.activeTextures[i].type;

            switch (type)
            {
            case Aozora::Material::DIFFUSE:
                textureUnit = 0;
                name = "texture_diffuse";
                break;
            case Aozora::Material::NORMAL:
                textureUnit = 1;
                name = "texture_normal";
                break;
            case Aozora::Material::EMISSIVE:
                textureUnit = 2;
                name = "texture_emissive";
                break;
            case Aozora::Material::AO:
                textureUnit = 3;
                name = "texture_ao";
                break;
            case Aozora::Material::METALLIC:
                textureUnit = 4;
                name = "texture_metallic";
                break;
            case Aozora::Material::ROUGHNESS:
                textureUnit = 5;
                name = "texture_roughness";
                break;
            default:
                continue;
                break;
            }

            glActiveTexture(GL_TEXTURE0 + textureUnit); // activate the texture
            // quite slow
            shader.setInt(("has_" + name), 1);
            shader.setInt(name, textureUnit);
            glBindTexture(GL_TEXTURE_2D, material.activeTextures[i].id);
        }


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, meshData.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // clean
        for (const auto& texture : material.activeTextures) {

            int textureUnit = 0;
            switch (texture.type) {
            case Aozora::Material::DIFFUSE:     textureUnit = 0; break;
            case Aozora::Material::NORMAL:      textureUnit = 1; break;
            case Aozora::Material::EMISSIVE:    textureUnit = 2; break;
            case Aozora::Material::AO:          textureUnit = 3; break;
            case Aozora::Material::METALLIC:    textureUnit = 4; break;
            case Aozora::Material::ROUGHNESS:   textureUnit = 5; break;
            default: continue;
            }
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }
    void Mesh::drawGeometry()
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, meshData.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }
}
