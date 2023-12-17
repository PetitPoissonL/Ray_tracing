//
// Created by Ningyu & Wenchi on 10/01/2022.
//

#ifndef LANCER_RAYONS_RAY_HPP
#define LANCER_RAYONS_RAY_HPP
#include "Vecteur3D.hpp"

class Ray {
public:
    // constructeur
    Ray() {}
    Ray(const Point3D& origin, const Vecteur3D& direction)
            : orig(origin), dir(direction)
    {}

    // Getter
    Point3D origin() const  { return orig; }
    Vecteur3D direction() const { return dir; }

    // Selon t, chercher un point dans le rayon : p(t) = origine + t*direction;
    Point3D at(double t) const {
        return orig + t*dir;
    }

public:
    Point3D orig;
    Vecteur3D dir;
};

#endif //LANCER_RAYONS_RAY_HPP
