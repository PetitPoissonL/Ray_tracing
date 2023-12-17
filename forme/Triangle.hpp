//
// Created by LI & Wang on 07/05/2022.
//

#ifndef RAY_TRACING_TRIANGLE_HPP
#define RAY_TRACING_TRIANGLE_HPP

#include "Touche.hpp"
#include "Vecteur3D.hpp"

class Triangle : public Touche {
public:
    Point3D p1, p2, p3;
    shared_ptr<Materiel> mate_ptr;
public:
    Triangle() = default;
    Triangle(Vecteur3D v1, Vecteur3D v2, Vecteur3D v3, shared_ptr<Materiel> mat)
            : p1(v1), p2(v2), p3(v3), mate_ptr(mat) {};
    virtual ~Triangle() = default;

    virtual bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const;
    virtual bool boundingBox(AABB &outputBox) const;
};

bool Triangle::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    Vecteur3D normal = (p2-p1)*(p3-p1);
    double n_dot_dir = produitScal(normal,r.direction());

    // non intersection
    if (n_dot_dir == 0.0)
        return false;
    double d = produitScal(-normal, p1);
    double t = -(produitScal(normal, r.origin())+d)/n_dot_dir;
    if ( t < t_min || t > t_max)
        return false;
    enreg.t = t;
    enreg.p = r.at(t);
    enreg.mate_ptr = mate_ptr;

    // Intérieur ou pas
    Vecteur3D pointR = enreg.p - p1;
    Vecteur3D q1 = p2 - p1;
    Vecteur3D q2 = p3 - p1;
    double q1LongueurCarre = q1.longueurCarree();
    double q2LongueurCarre = q2.longueurCarree();
    double q1_dot_q2 = produitScal(q1, q2);
    double pointR_dot_q1 = produitScal(pointR, q1);
    double pointR_dot_q2 = produitScal(pointR, q2);
    double determinant = 1.0/(q1LongueurCarre*q2LongueurCarre-q1_dot_q2*q1_dot_q2);

    double omega1 = determinant*(q2LongueurCarre*pointR_dot_q1-q1_dot_q2*pointR_dot_q2);
    double omega2 = determinant*(q1LongueurCarre*pointR_dot_q2-q1_dot_q2*pointR_dot_q1);
    if (omega1+omega2 > 1.0 || omega1 < 0.0 || omega2 < 0.0)
        return false;
    enreg.normal =
}

#endif //RAY_TRACING_TRIANGLE_HPP
