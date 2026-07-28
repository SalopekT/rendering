#include "Object.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <glad/glad.h>
Object::Object(const std::vector<Mesh>& meshes) : meshes(meshes), textureId(0){
    for (Mesh& mesh : this->meshes) {
        mesh.createBuffer();
    }
}

Object::Object(const std::string& pathToModel, const std::string& pathToTexture) : textureId(0),pathToModel(pathToModel), pathToTexture(pathToTexture){

    // https://learnopengl.com/Model-Loading/Model
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pathToModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    this->processNode(scene->mRootNode, scene);

}

void Object::processNode(aiNode* node, const aiScene* scene) {
    //node->mMeshes contains indices(integers) of meshes in scene array of meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh myMesh = Mesh::processMesh(mesh, scene);
        myMesh.createBuffer();
        meshes.push_back(myMesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Object Object::copy() {
    return *this;
}

void Object::print() const {
    for (int i=0;i<this->meshes.size();i++) {
        std::cout << "Mesh n: " << i << "\n";
        this->meshes.at(i).print();
    }

}

Mesh& Object::getMesh(int i) {
    return this->meshes.at(i);
}


float* Object::getVerticesArray() {
    //float* arr = new float[this->vertices.size() * 3];
    return nullptr;
}


unsigned int Object::generateTexture() {
    //generating a texture
    int width1, height1, nrChannels;
    unsigned char* data = stbi_load(this->pathToTexture.c_str(), &width1, &height1, &nrChannels, 0);

    if (data) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        //selecting a texture unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        this->textureId = texture;
        return texture;
    }
    return 0;
}
void Object::setTransform() {

}
void Object::setMaterialCoeffs(glm::vec3 materialCoeffs) {
    this->materialCoeffs = materialCoeffs;
}

void setMaterialCoeffs();
Transform Object::getTransform() {
    return this->transform;
}
glm::vec3 Object::getMaterialCoeffs() {
    return this->materialCoeffs;
}
bool Object::hasTexture() {
    return this->hasTextureFlag;
}
void Object::drawObject() {
    for (Mesh& mesh : this->meshes) {
        if (this->textureId != 0) glBindTexture(GL_TEXTURE_2D, this->textureId);
        mesh.drawMesh();
    }

}

