//
// Created by LI & WANG on 02/05/2022.
//

#ifndef RAY_TRACING_ONB_HPP
#define RAY_TRACING_ONB_HPP

#include "Vecteur3D.hpp"
// ONB : orthonormal basis, a collection of three mutually orthogonal unit vectors.

class Onb {
public:
    Vecteur3D axe[3];

public:
    Onb() {}

    inline Vecteur3D operator[](int i) const {
        return axe[i];
    }

    Vecteur3D u() const { return axe[0]; }
    Vecteur3D v() const { return axe[1]; }
    Vecteur3D w() const { return axe[2]; }

    Vecteur3D local(double a, double b, double c) const {
        return a*u()+b*v()+c*w();
    }

    Vecteur3D local(const Vecteur3D &a) const {
        return a.getX()*u()+a.getY()*v()+a.getZ()*w();
    }

    void build_from_w(const Vecteur3D&);
};

void Onb::build_from_w(const Vecteur3D &n) {
    axe[2] = normalisation(n);
    Vecteur3D a = (fabs(w().getX()) > 0.9) ? Vecteur3D(0,1,0) : Vecteur3D(1,0,0);
    axe[1] = normalisation(produitVect(w(), a));
    axe[0] = produitVect(w(), v());
}

#endif //RAY_TRACING_ONB_HPP
