//
// Created by Ningyu & Wenchi on 14/03/2022.
//

#ifndef RAY_TRACING_DEPLACEMENT_HPP
#define RAY_TRACING_DEPLACEMENT_HPP

#include "Touche.hpp"

class Deplacement : public Touche {
public:
    shared_ptr<Touche> ptr;
    Vecteur3D decalage;

public:
    Deplacement(shared_ptr<Touche> p, const Vecteur3D& deplacement) : ptr(p), decalage(deplacement){}
    virtual ~Deplacement() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB& outputBox) const override;
};

bool Deplacement::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    Ray deplaceRay(r.origin()-decalage, r.direction());
    if(!ptr->touche(deplaceRay, t_min, t_max, enreg))
        return false;

    enreg.p = enreg.p + decalage;
    enreg.set_face_normal(deplaceRay, enreg.normal);

    return true;
}

bool Deplacement::boundingBox(AABB &outputBox) const {
    if(!ptr->boundingBox(outputBox))
        return false;

    outputBox = AABB(outputBox.min()+decalage, outputBox.max()+decalage);

    return true;
}

#endif //RAY_TRACING_DEPLACEMENT_HPP
