#ifndef MESH_HPP
#define MESH_HPP
#include <string>
#include "Face3d.hpp"
#include "Vertex3d.hpp"
#include "Shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Mesh{
    private:
        std::vector<std::shared_ptr<Vertex3d>> vertices;
        std::vector<Face3d> faces;
        std::vector<unsigned int> indices;

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

    public:
        Mesh(std::vector<std::shared_ptr<Vertex3d>> vertices, std::vector<Face3d> faces, std::vector<unsigned int> indices);
        ~Mesh() {}

        static Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        float* getVerticesArray();
        int getNumOfVertices();

        unsigned int createBuffer(); //generates VBO and VAO, returns id of VAO
        void drawMesh();

        void print() const;
        unsigned int getVAO();
        unsigned int getVBO();
        unsigned int getEBO();
        
};

#endif


