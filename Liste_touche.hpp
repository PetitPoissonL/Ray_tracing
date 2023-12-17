//
// Created by Ningyu & Wenchi on 05/02/2022.
//

#ifndef LANCER_RAYONS_LISTE_TOUCHE_HPP
#define LANCER_RAYONS_LISTE_TOUCHE_HPP

#include "Outils.hpp"
#include "Touche.hpp"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Liste_touche : public Touche {
public:
    std::vector<shared_ptr<Touche>> objects;

public:
    Liste_touche()= default;
    explicit Liste_touche(const shared_ptr<Touche>& object) {add(object);}
    virtual ~Liste_touche() = default;

    void clear(){ objects.clear();}
    void add(const shared_ptr<Touche>& object) { objects.push_back(object);}
    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& rec) const override;
    bool boundingBox(AABB& outputBox) const override;
    double pdf_valeur(const Point3D &o, const Vecteur3D &v) const override;
    Vecteur3D random(const Vecteur3D &o) const override;
};

bool Liste_touche::touche(const Ray &r, double t_min, double t_max, touche_enreg &rec) const {
    touche_enreg temp_enreg;
    bool touche_qcq = false; // touché quelque chose
    auto plusProche = t_max;

    for (const auto& object : objects) {
        if (object->touche(r, t_min, plusProche, temp_enreg)){
            touche_qcq = true;
            plusProche = temp_enreg.t;
            rec = temp_enreg;
        }
    }

    return touche_qcq;
}

bool Liste_touche::boundingBox(AABB &outputBox) const {
    if(objects.empty()) return false;
    AABB tmpBox;
    bool premierBox = true;
    for(const auto& object: objects) {
        if (!object->boundingBox(tmpBox))
            return false;
        outputBox = premierBox ? tmpBox : surroundingBox(outputBox, tmpBox);
        premierBox = false;
    }
    return true;
}

double Liste_touche::pdf_valeur(const Point3D &o, const Vecteur3D &v) const {
    auto weight = 1.0/objects.size();
    auto sum = 0.0;

    for (const auto& object : objects)
        sum += weight * object->pdf_valeur(o, v);

    return sum;
}

Vecteur3D Liste_touche::random(const Vecteur3D &o) const {
    double int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size-1)]->random(o);
}

#endif //LANCER_RAYONS_LISTE_TOUCHE_HPP
