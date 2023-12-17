//
// Created by Ningyu & Wenchi on 05/02/2022.
//

#ifndef LANCER_RAYONS_TOUCHE_HPP
#define LANCER_RAYONS_TOUCHE_HPP

#include "Outils.hpp"
#include "AABB.hpp"

class Materiel;

// Enregistre les informations de peut-être touché dans une structure
struct touche_enreg{
    Point3D p;
    Vecteur3D normal;
    double t; // la touche ne compte que si t_min < t < t_max
    double u;
    double v;
    bool front_face;
    shared_ptr<Materiel> mate_ptr;

    // Configurer que les normales pointent toujours "vers l'extérieur" de la surface
    inline void set_face_normal(const Ray& r, const Vecteur3D& exterieur_normal){
        front_face = produitScal(r.direction(), exterieur_normal) < 0;
        normal = front_face ? exterieur_normal : -exterieur_normal;
    }
};

class Touche{
public:
    virtual bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const = 0;
    virtual bool boundingBox(AABB& outputBox) const = 0;
    virtual double pdf_valeur(const Point3D &o, const Vecteur3D &v) const { return 0.0; }
    virtual Vecteur3D random(const Vecteur3D &o) const { return {1,0,0}; }
};

// La lumière est à double face. Nous voulons que la lumière soit émise vers le bas
class Face_retourne : public Touche {
public:
    shared_ptr<Touche> ptr;

public:
    explicit Face_retourne(shared_ptr<Touche> p) : ptr(p) {}
    virtual ~Face_retourne() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& enreg) const override {
        if (!ptr->touche(r, t_min, t_max, enreg))
            return false;

        enreg.front_face = !enreg.front_face;
        return true;
    }

    bool boundingBox(AABB& outputBox) const override {
        return ptr->boundingBox(outputBox);
    }
};

#endif //LANCER_RAYONS_TOUCHE_HPP
