#include "Face3d.hpp"
#include <iostream>
#include <glm/glm.hpp>

Face3d::Face3d(std::shared_ptr<Vertex3d> v1, std::shared_ptr<Vertex3d> v2, std::shared_ptr<Vertex3d> v3) : v1(v1), v2(v2), v3(v3){
    glm::vec3 sub1 = v2->getPosition() - v1->getPosition();
    glm::vec3 sub2 = v3->getPosition() - v1->getPosition();

    glm::vec3 normal = glm::cross(sub1, sub2);
    a = normal[0];
    b = normal[1];
    c = normal[2];
    d = -a * v2->getPosition()[0] - b * v2->getPosition()[1] - c * v2->getPosition()[2];
};

void Face3d::print() const{
    std::cout << "First: ";
    v1->print();
    std::cout << "    Second: " ;
    v2->print();
    std::cout << "    Third: ";
    v3->print();
    std::cout << std::endl;
}

//0 za ispod, 1 za inad, 2 za na plohi
int Face3d::pointCheck(double x, double y, double z) const{
    double result = this->a*x+this->b*y+this->c*z+this->d;
    if (result<-1e-7){
        return 0;
    }
    else if(result>1e-7){
        return 1;
    }
    return 2;
}


void Face3d::recomputePlane() {
    //same as in constructor
    glm::vec3 sub1 = v2->getPosition() - v1->getPosition();
    glm::vec3 sub2 = v3->getPosition() - v1->getPosition();

    glm::vec3 normal = glm::cross(sub1, sub2);
    a = normal[0];
    b = normal[1];
    c = normal[2];
    d = -a * v2->getPosition()[0] - b * v2->getPosition()[1] - c * v2->getPosition()[2];
}

std::shared_ptr<Vertex3d> Face3d::getVertex1() const{
    return v1;
}

std::shared_ptr<Vertex3d> Face3d::getVertex2() const{
    return v2;
}

std::shared_ptr<Vertex3d> Face3d::getVertex3() const{
    return v3;
}

void Face3d::setVisible(bool isVisible){
    this->isVisible=isVisible;
}

glm::vec3 Face3d::getNormal() const{
    glm::vec3 normal(a,b,c);
    return normal;
}

bool Face3d::getVisibility() const{
    return isVisible;
}