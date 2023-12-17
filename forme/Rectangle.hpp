//
// Created by Ningyu & Wenchi on 13/03/2022.
//

#ifndef RAY_TRACING_RECTANGLE_HPP
#define RAY_TRACING_RECTANGLE_HPP

#include "../Outils.hpp"
#include "../Touche.hpp"

class RectangleXY : public Touche {
public:
    double x0, x1, y0, y1, k;
    shared_ptr<Materiel> m;

public:
    RectangleXY() = default;
    RectangleXY(double x_0, double x_1, double y_0, double y_1, double val, shared_ptr<Materiel> mat)
        : x0(x_0), x1(x_1), y0(y_0), y1(y_1), k(val), m(mat) {};
    virtual ~RectangleXY() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB& outputBox) const override{
        outputBox = AABB(Point3D(x0, y0, k-0.0001), Point3D(x1, y1, k+0.0001));
        return true;
    }
};

class RectangleXZ : public Touche {
public:
    double x0, x1, z0, z1, k;
    shared_ptr<Materiel> m;

public:
    RectangleXZ() = default;
    RectangleXZ(double x_0, double x_1, double z_0, double z_1, double val, shared_ptr<Materiel> mat)
            : x0(x_0), x1(x_1), z0(z_0), z1(z_1), k(val), m(mat) {};
    virtual ~RectangleXZ() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB& outputBox) const override{
        outputBox = AABB(Point3D(x0, k-0.0001, z0), Point3D(x1, k+0.0001, z1));
        return true;
    }

    double pdf_valeur(const Point3D &o, const Vecteur3D &v) const override {
        touche_enreg enreg;
        if (!this->touche(Ray(o, v), 0.001, infini, enreg))
            return 0;

        double zone = (x1-x0)*(z1-z0);
        double distanceCarree = enreg.t * enreg.t * v.longueurCarree();
        double cos = fabs(produitScal(v, enreg.normal)/v.longueur());

        return distanceCarree/(cos*zone);
    }

    Vecteur3D random(const Vecteur3D &o) const override {
        Point3D randomPoint = Point3D(random_double(x0, x1), k, random_double(z0, z1));
        return randomPoint - o;
    }
};

class RectangleYZ : public Touche {
public:
    double y0, y1, z0, z1, k;
    shared_ptr<Materiel> m;

public:
    RectangleYZ() = default;
    RectangleYZ(double y_0, double y_1, double z_0, double z_1, double val, shared_ptr<Materiel> mat)
            : y0(y_0), y1(y_1), z0(z_0), z1(z_1), k(val), m(mat) {};
    virtual ~RectangleYZ() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB& outputBox) const override{
        outputBox = AABB(Point3D(k-0.0001, y0, z0), Point3D(k+0.0001, y1, z1));
        return true;
    }
};

bool RectangleXY::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    auto t = (k-r.origin().getZ()) / r.direction().getZ();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().getX() + t*r.direction().getX();
    auto y = r.origin().getY() + t*r.direction().getY();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    enreg.u = (x-x0)/(x1-x0);
    enreg.v = (y-y0)/(y1-y0);
    enreg.t = t;
    auto outward_normal = Vecteur3D(0, 0, 1);
    enreg.set_face_normal(r, outward_normal);
    enreg.mate_ptr = m;
    enreg.p = r.at(t);
    return true;
}

bool RectangleXZ::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    auto t = (k-r.origin().getY()) / r.direction().getY();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().getX() + t*r.direction().getX();
    auto z = r.origin().getZ() + t*r.direction().getZ();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    enreg.u = (x-x0)/(x1-x0);
    enreg.v = (z-z0)/(z1-z0);
    enreg.t = t;
    auto outward_normal = Vecteur3D(0, 1, 0);
    enreg.set_face_normal(r, outward_normal);
    enreg.mate_ptr = m;
    enreg.p = r.at(t);
    return true;
}

bool RectangleYZ::touche(const Ray &r, double t_min, double t_max, touche_enreg &enreg) const {
    auto t = (k-r.origin().getX()) / r.direction().getX();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().getY() + t*r.direction().getY();
    auto z = r.origin().getZ() + t*r.direction().getZ();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    enreg.u = (y-y0)/(y1-y0);
    enreg.v = (z-z0)/(z1-z0);
    enreg.t = t;
    auto outward_normal = Vecteur3D(1, 0, 0);
    enreg.set_face_normal(r, outward_normal);
    enreg.mate_ptr = m;
    enreg.p = r.at(t);
    return true;
}

#endif //RAY_TRACING_RECTANGLE_HPP
