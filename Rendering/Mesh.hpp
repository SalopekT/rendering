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

        unsigned int VAO;

    public:
        Mesh(std::vector<std::shared_ptr<Vertex3d>> vertices, std::vector<Face3d> faces);
        ~Mesh() {}

        static Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        float* getVerticesArray();
        int getNumOfVertices();

        unsigned int createBuffer(); //generates VBO and VAO, returns id of VAO
        void drawMesh(ShaderProgramme* sp, glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectionMat);

        void print() const;
        
};

#endif


