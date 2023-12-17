//
// Created by Ningyu & Wenchi on 05/02/2022.
//

#ifndef LANCER_RAYONS_SPHERE_HPP
#define LANCER_RAYONS_SPHERE_HPP

#include <utility>

#include "../Touche.hpp"
#include "../Vecteur3D.hpp"
#include "../ONB.hpp"

class Sphere : public Touche{
public:
    Point3D centre;
    double radius{};
    shared_ptr<Materiel> mate_ptr;
public:

    Sphere()= default;
    Sphere(Point3D cen, double r, shared_ptr<Materiel> m) : centre(cen), radius(r) , mate_ptr(m){}
    virtual ~Sphere() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override;
    bool boundingBox(AABB &outputBox) const override;

    double pdf_valeur(const Point3D &o, const Vecteur3D &v) const override {
        touche_enreg enreg;
        if (!this->touche(Ray(o, v), 0.001, infini, enreg))
            return 0;
        double cos_theta_max = sqrt(1-radius*radius/(centre-o).longueurCarree());
        double solideAngle = 2*pi*(1-cos_theta_max);
        return 1/solideAngle;
    }

    Vecteur3D random(const Vecteur3D &o) const override {
        Vecteur3D direction = centre - o;
        double distanceCarree = direction.longueurCarree();
        Onb uvw;
        uvw.build_from_w(direction);
        return uvw.local(random_sphere(radius, distanceCarree));
    }

private:
    static void getSphereUV(const Point3D& p, double& u, double& v){
        double theta = acos(-p.getY());
        double phi = atan2(-p.getZ(), p.getX()) + pi;
        u = phi / (2*pi);
        v = theta / pi;
    }
};

bool Sphere::touche(const Ray &r, double t_min, double t_max, touche_enreg& enreg) const {
    // s'il getY a l'intersection entre Ray et Sphere : b*b-4*a*c >= 0
    // equation de Ray : P(t) = A + tB
    // equation de Sphere : (P-C)(P-C) = R*R
    // equation d'intersection : B*B*t*t + 2tB(A-C) + (A-C)(A-C)-R*R = 0
    Vecteur3D oc = r.origin() - centre;
    auto a = r.direction().longueurCarree();
    auto demi_b = produitScal(oc, r.direction());
    auto c = produitScal(oc, oc) - radius * radius;
    auto discriminant = demi_b * demi_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Trouvez la racine la plus proche qui se trouve dans l'intervalle acceptable
    auto racine = (-demi_b - sqrtd) / a;
    if (racine < t_min || t_max < racine){
        racine = (-demi_b + sqrtd) / a;
        if (racine < t_min || t_max < racine)
            return false;
    }

    enreg.t = racine;
    enreg.p = r.at(enreg.t);
    Vecteur3D exterieur_normal = (enreg.p - centre) / radius;
    enreg.set_face_normal(r, exterieur_normal);
    getSphereUV(exterieur_normal, enreg.u, enreg.v);
    enreg.mate_ptr = mate_ptr;

    return true;
}

bool Sphere::boundingBox(AABB &outputBox) const {
    outputBox = AABB(centre-Vecteur3D(radius, radius, radius),
                     centre+Vecteur3D(radius,radius,radius));
    return true;
}

#endif //LANCER_RAYONS_SPHERE_HPP
