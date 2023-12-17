//
// Created by Ningyu & Wenchi on 04/01/2022.
//

#ifndef LANCER_RAYONS_VECTEUR3D_HPP
#define LANCER_RAYONS_VECTEUR3D_HPP
#include <iostream>
#include <cmath>
#include "Outils.hpp"

using std::sqrt;

class Vecteur3D {
public:
    double v[3];
public:
    Vecteur3D() : v{0, 0, 0} {}
    Vecteur3D(double v0, double v1, double v2) : v{v0,v1,v2} {}

    double getX() const { return v[0]; }
    double getY() const { return v[1]; }
    double getZ() const { return v[2]; }

    Vecteur3D operator-() const { return {-v[0], -v[1], -v[2]}; }
    double operator[](int i) const { return v[i]; }
    double& operator[](int i) { return v[i]; }

    double longueur() const {
        return sqrt(longueurCarree());
    }

    double longueurCarree() const {
        return v[0]*v[0] + v[1] * v[1] + v[2] * v[2];
    }

    bool proche_zero() const {
        const double val = 1e-8;
        return ((fabs(v[0]) < val) && (fabs(v[1]) < val) && (fabs(v[2]) < val)) ;
    }
};

// Type d'alias pour Vecteur3D
using Point3D = Vecteur3D;  // Point 3D
using Color = Vecteur3D;    // Couleur RGB

inline std::ostream& operator<<(std::ostream &out, const Vecteur3D &v) {
    return out << v.v[0] << ' ' << v.v[1] << ' ' << v.v[2];
}

// Opérateurs surchargés
inline Vecteur3D operator+(const Vecteur3D &u, const Vecteur3D &v) {
    return {u.v[0] + v.v[0], u.v[1] + v.v[1], u.v[2] + v.v[2]};
}

inline Vecteur3D operator-(const Vecteur3D &u, const Vecteur3D &v) {
    return {u.v[0] - v.v[0], u.v[1] - v.v[1], u.v[2] - v.v[2]};
}

inline Vecteur3D operator*(const Vecteur3D &u, const Vecteur3D &v) {
    return {u.v[0] * v.v[0], u.v[1] * v.v[1], u.v[2] * v.v[2]};
}

inline Vecteur3D operator*(double t, const Vecteur3D &v) {
    return {t*v.v[0], t*v.v[1], t * v.v[2]};
}

inline Vecteur3D operator*(const Vecteur3D &v, double t) {
    return t * v;
}

inline Vecteur3D operator/(Vecteur3D v, double t) {
    return (1/t) * v;
}

// Produit scalaire
inline double produitScal(const Vecteur3D &u, const Vecteur3D &v) {
    return u.v[0] * v.v[0]
           + u.v[1] * v.v[1]
           + u.v[2] * v.v[2];
}


inline Vecteur3D produitVect(const Vecteur3D &u, const Vecteur3D &v) {
    return {u.v[1] * v.v[2] - u.v[2] * v.v[1],
            u.v[2] * v.v[0] - u.v[0] * v.v[2],
            u.v[0] * v.v[1] - u.v[1] * v.v[0]};
}

// normalisation
inline Vecteur3D normalisation(Vecteur3D v) {
    return v / v.longueur();
}

// vecteur random
inline Vecteur3D vecteurRandom() {
    return {random_double(), random_double(), random_double()};
}

inline Vecteur3D vecteurRandom(double min, double max) {
    return {random_double(min, max), random_double(min, max), random_double(min, max)};
}

// Un point random dans une sphere unit
inline Vecteur3D vectRandomASphereUnit() {
    while(true) {
        Point3D p = vecteurRandom(-1,1);
        if (p.longueurCarree() >= 1)
            continue;
        return p;
    }
}

inline Vecteur3D vecteurRandomUnit() {
    return normalisation(vectRandomASphereUnit());
}

// Retourner une direction réfléchie. (u est la direction incidente, v est un vecteur unitaire)
inline Vecteur3D refleter(const Vecteur3D &u, const Vecteur3D &v) {
    return u-2*produitScal(u, v)*v;
}

inline Vecteur3D refracter(const Vecteur3D &u, const Vecteur3D &n, double refract_ratio) {
    double cosTheta = fmin(produitScal(-u, n), 1.0);
    Vecteur3D rayPerpend = refract_ratio * (u + cosTheta * n);
    Vecteur3D rayParallel = -sqrt(fabs(1.0 - rayPerpend.longueurCarree())) * n;
    return rayPerpend + rayParallel;
}

inline Vecteur3D randomInUnitDisk() {
    while (true) {
        auto p = Vecteur3D(random_double(-1,1), random_double(-1,1), 0);
        if (p.longueurCarree() >= 1) continue;
        return p;
    }
}

inline Vecteur3D random_cosine_direction() {
    double r1 = random_double();
    double r2 = random_double();
    double z = sqrt(1-r2);

    double phi = 2*pi*r1;
    double x = cos(phi)*sqrt(r2);
    double y = sin(phi)*sqrt(r2);

    return {x, y, z};
}

inline Vecteur3D random_sphere(double radius, double distanceCarree) {
    double r1 = random_double();
    double r2 = random_double();
    double z = 1+r2*(sqrt(1-radius*radius/distanceCarree)-1);
    double phi = 2*pi*r1;
    double x = cos(phi)* sqrt(1-z*z);
    double y = sin(phi)* sqrt(1-z*z);
    return {x,y,z};
}

#endif //LANCER_RAYONS_VECTEUR3D_HPP
