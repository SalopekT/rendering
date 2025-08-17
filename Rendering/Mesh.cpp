#include "Mesh.hpp"
#include "Shader.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>

Mesh::Mesh(std::vector<std::shared_ptr<Vertex3d>> vertices, std::vector<Face3d> faces) : vertices(vertices), faces(faces) {}

Mesh Mesh::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<std::shared_ptr<Vertex3d>> vertices;
    std::vector<Face3d> faces;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec2 texCoords(0.0f, 0.0f); // default if no UVs
        if (mesh->HasTextureCoords(0)) {
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            //std::cout << texCoords.x << " " << texCoords.y << "\n";
        }
        std::shared_ptr<Vertex3d> vertex = std::make_shared<Vertex3d>(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 
                                                                      mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z,
                                                                      texCoords.x, texCoords.y);
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        std::vector<unsigned int> indices;
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            unsigned int vertexIndex = mesh->mFaces[i].mIndices[j];
            indices.push_back(vertexIndex);
        }
        //suppose i only have triangles in a mesh
        Face3d face(vertices.at(indices.at(0)), vertices.at(indices.at(1)), vertices.at(indices.at(2)));
        faces.push_back(face);
    }


    return Mesh(vertices, faces);
}

//this returns not just positions, but normals and other stuff
float* Mesh::getVerticesArray() {
    float* arr = new float[this->vertices.size()*8];
    for (int i=0;i<this->vertices.size();i++) {
        arr[i * 8] = this->vertices.at(i)->getPosition()[0];
        arr[i*8+1] = this->vertices.at(i)->getPosition()[1];
        arr[i*8+2] = this->vertices.at(i)->getPosition()[2];

        arr[i * 8+3] = this->vertices.at(i)->getNormal()[0];
        arr[i * 8 + 4] = this->vertices.at(i)->getNormal()[1];
        arr[i * 8 + 5] = this->vertices.at(i)->getNormal()[2];

        arr[i * 8 + 6] = this->vertices.at(i)->getUVs()[0];
        arr[i * 8 + 7] = this->vertices.at(i)->getUVs()[1];
        

        //v1.x,v1.y,v1.z,v1.a,v1.b,v1.c,v1.u,v1.v,v2.x,v2,y,...
    }
    return arr;
}


void Mesh::print() const {
    std::cout << "Vertices: \n";
    for (int i=0;i<this->vertices.size();i++) {
        this->vertices.at(i)->print();
    }

}



int Mesh::getNumOfVertices() {
    return this->vertices.size();
}

unsigned int Mesh::createBuffer() {
    //generating and binding vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //generating vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->getNumOfVertices() * 8 * sizeof(float), this->getVerticesArray(), GL_STATIC_DRAW);

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    this->VAO = VAO;
    return VAO;
}

void Mesh::drawMesh(ShaderProgramme* sp, glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectionMat) {
    glBindVertexArray(this->VAO);
    sp->setUniformsVertexShader(modelMat, viewMat, projectionMat);
    glDrawArrays(GL_TRIANGLES, 0, this->getNumOfVertices());
}