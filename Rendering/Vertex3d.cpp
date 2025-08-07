#include "Vertex3d.hpp"
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>

Vertex3d::Vertex3d(double x, double y, double z){
    this->position = glm::vec3(x, y, z);
}

void Vertex3d::print() const{
    std::cout << std::setprecision(4) << "X: " << this->position[0] << " Y: " << this->position[1] << " Z: " << this->position[2] << std::endl;
}


void Vertex3d::setX(double x){
    this->position[0] = x;
}

void Vertex3d::setY(double y){
    this->position[1] = y;
}

void Vertex3d::setZ(double z){
    this->position[2] = z;
}

void Vertex3d::setPosition(glm::vec3 newPosition) {
    this->position = newPosition;
}

glm::vec3 Vertex3d::getPosition() {
    return this->position;
}
     

void Vertex3d::setA(double a) {
    this->normal[0] = a;
}
void Vertex3d::setB(double b) {
    this->normal[1] = b;
}
void Vertex3d::setC(double c) {
    this->normal[2] = c;
}

void Vertex3d::setNormal(glm::vec3 newNormal) {
    this->normal = newNormal;
}
       
glm::vec3 Vertex3d::getNormal(){
    return normal;
}