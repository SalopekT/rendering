#ifndef MESH_HPP
#define MESH_HPP
#include <string>
#include "Face3d.hpp"
#include "Vertex3d.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Mesh{
    private:
        std::vector<std::shared_ptr<Vertex3d>> vertices;
        std::vector<Face3d> faces;

    public:
        Mesh(std::vector<std::shared_ptr<Vertex3d>> vertices, std::vector<Face3d> faces);
        ~Mesh() {}

        static Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        void print() const;
        
};

#endif


