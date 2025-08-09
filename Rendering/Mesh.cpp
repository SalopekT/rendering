#include "Mesh.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(std::vector<std::shared_ptr<Vertex3d>> vertices, std::vector<Face3d> faces) : vertices(vertices), faces(faces) {}

Mesh Mesh::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<std::shared_ptr<Vertex3d>> vertices;
    std::vector<Face3d> faces;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        std::shared_ptr<Vertex3d> vertex = std::make_shared<Vertex3d>(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
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

float* Mesh::getVerticesArray() {
    float* arr = new float[this->vertices.size()*3];
    for (int i=0;i<this->vertices.size();i++) {
        arr[i * 3] = this->vertices.at(i)->getPosition()[0];
        arr[i*3+1] = this->vertices.at(i)->getPosition()[1];
        arr[i*3+2] = this->vertices.at(i)->getPosition()[2];
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