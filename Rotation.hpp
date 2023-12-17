//
// Created by Ningyu & Wenchi on 14/03/2022.
//

#ifndef RAY_TRACING_ROTATION_HPP
#define RAY_TRACING_ROTATION_HPP

#include "Touche.hpp"
// y-rotation
class Rotation : public Touche {
public:
    shared_ptr<Touche> ptr;
    double sinTheta;
    double cosTheta;
    bool hasBox;
    AABB aabbbox;

public:
    Rotation(shared_ptr<Touche> p, double angle);
    virtual ~Rotation() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB& outputBox) const override {
        outputBox = aabbbox;
        return hasBox;
    }
};

Rotation::Rotation(shared_ptr<Touche> p, double angle) : ptr(p) {
    double radians = degresARadians(angle);
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasBox = ptr->boundingBox(aabbbox);
    Point3D min(infini, infini, infini);
    Point3D max(-infini, -infini, -infini);

    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            for(int k=0; k<2; k++){
                double x = i*aabbbox.max().getX() + (1-i)*aabbbox.min().getX();
                double y = j*aabbbox.max().getY() + (1-j)*aabbbox.min().getY();
                double z = k*aabbbox.max().getZ() + (1-k)*aabbbox.min().getZ();

                double newX = cosTheta*x+sinTheta*z;
                double newZ = -sinTheta*x+cosTheta*z;
                Vecteur3D tester(newX, y, newZ);

                for(int c=0; c<3; c++){
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }
    aabbbox = AABB(min, max);
}

bool Rotation::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    Vecteur3D origine = r.origin();
    Vecteur3D direction = r.direction();
    origine[0] = cosTheta*r.origin()[0] - sinTheta*r.origin()[2];
    origine[2] = sinTheta*r.origin()[0] + cosTheta*r.origin()[2];
    direction[0] = cosTheta*r.direction()[0] - sinTheta*r.direction()[2];
    direction[2] = sinTheta*r.direction()[0] + cosTheta*r.direction()[2];
    Ray rorationRay(origine, direction);

    if(!ptr->touche(rorationRay, t_min, t_max, enreg))
        return false;

    Vecteur3D p = enreg.p;
    Vecteur3D normal = enreg.normal;
    p[0] = cosTheta*enreg.p[0] + sinTheta*enreg.p[2];
    p[2] = -sinTheta*enreg.p[0] + cosTheta*enreg.p[2];
    normal[0] = cosTheta*enreg.normal[0] + sinTheta*enreg.normal[2];
    normal[2] = -sinTheta*enreg.normal[0] + cosTheta*enreg.normal[2];
    enreg.p = p;
    enreg.set_face_normal(rorationRay, normal);

    return true;
}
#endif //RAY_TRACING_ROTATION_HPP
