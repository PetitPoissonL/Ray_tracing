//
// Created by Ningyu & Wenchi on 14/03/2022.
//

#ifndef RAY_TRACING_BOX_HPP
#define RAY_TRACING_BOX_HPP

#include "Outils.hpp"
#include "forme/Rectangle.hpp"
#include "Liste_touche.hpp"

class Box : public Touche {
public:
    Point3D boxMin;
    Point3D boxMax;
    Liste_touche faces;

public:
    Box(){}
    Box(const Point3D& p0, const Point3D& p1, shared_ptr<Materiel> ptr);
    virtual ~Box() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB& outputBox) const override{
        outputBox = AABB(boxMin, boxMax);
        return true;
    }
};

Box::Box(const Point3D &p0, const Point3D &p1, shared_ptr<Materiel> ptr) {
    boxMin = p0;
    boxMax = p1;

    faces.add(make_shared<RectangleXY>(p0.getX(), p1.getX(), p0.getY(), p1.getY(), p1.getZ(), ptr));
    faces.add(make_shared<RectangleXY>(p0.getX(), p1.getX(), p0.getY(), p1.getY(), p0.getZ(), ptr));
    faces.add(make_shared<RectangleXZ>(p0.getX(), p1.getX(), p0.getZ(), p1.getZ(), p1.getY(), ptr));
    faces.add(make_shared<RectangleXZ>(p0.getX(), p1.getX(), p0.getZ(), p1.getZ(), p0.getY(), ptr));
    faces.add(make_shared<RectangleYZ>(p0.getY(), p1.getY(), p0.getZ(), p1.getZ(), p1.getX(), ptr));
    faces.add(make_shared<RectangleYZ>(p0.getY(), p1.getY(), p0.getZ(), p1.getZ(), p0.getX(), ptr));
}

bool Box::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    return faces.touche(r, t_min, t_max, enreg);
}

#endif //RAY_TRACING_BOX_HPP
