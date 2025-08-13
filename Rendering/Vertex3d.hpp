#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <glm/glm.hpp>
class Vertex3d{
    private:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    public:
        Vertex3d(double, double, double, double, double, double, double, double);
        ~Vertex3d() {}

        void print() const;
        
        void setX(double);
        void setY(double);
        void setZ(double);
        void setPosition(glm::vec3 newPosition);
        glm::vec3 getPosition();

        
        void setA(double);
        void setB(double);
        void setC(double);
        void setNormal(glm::vec3 newNormal);


        glm::vec3 getNormal();




};

#endif