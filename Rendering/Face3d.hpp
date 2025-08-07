#ifndef FACE_HPP
#define FACE_HPP
#include "Vertex3d.hpp"
#include <memory>


class Face3d{
    private:
        std::shared_ptr<Vertex3d> v1;
        std::shared_ptr<Vertex3d> v2;
        std::shared_ptr<Vertex3d> v3;
        // plane equation coefficients
        double a;
        double b;
        double c;
        double d;

        bool isVisible;

    public:
        Face3d(std::shared_ptr<Vertex3d>, std::shared_ptr<Vertex3d>, std::shared_ptr<Vertex3d>);
        ~Face3d() {}

        void print() const;
        //0 za ispod, 1 za inad, 2 za na plohi
        int pointCheck(double x, double y, double z) const;
        void recomputePlane();
        std::shared_ptr<Vertex3d> getVertex1() const;
        std::shared_ptr<Vertex3d> getVertex2() const;
        std::shared_ptr<Vertex3d> getVertex3() const;
        void setVisible(bool isVisible);
        bool getVisibility() const;

        glm::vec3 getNormal() const;
};

#endif