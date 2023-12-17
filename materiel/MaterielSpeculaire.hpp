//
// Created by 李宁豫 on 11/02/2022.
//

#ifndef RAY_TRACING_MATERIELSPECULAIRE_HPP
#define RAY_TRACING_MATERIELSPECULAIRE_HPP

#include "Materiel.hpp"

class MaterielSpeculaire : public Materiel {
public:
    Color albedo;
    double flou; // Plus l'objet est grand, plus les réflexions seront floues

public:
    explicit MaterielSpeculaire(const Color &a, double f) : albedo(a), flou(f < 1 ? f : 1){}
    virtual ~MaterielSpeculaire() = default;

    // Pour les métaux lisses, l'angle d'incidence est égal à l'angle de réflexion.
    bool diffuser(const Ray &r, const touche_enreg &enreg, diffuse_enreg &dEnreg) const override{
        Vecteur3D reflete_direct = refleter(normalisation(r.direction()), enreg.normal);
        dEnreg.raySpeculaire = Ray(enreg.p, reflete_direct+flou*vectRandomASphereUnit());
        dEnreg.atenuation = albedo;
        dEnreg.isSpeculaire = true;
        dEnreg.ptr_pdf = nullptr;
        return true;
    }
};

#endif //RAY_TRACING_MATERIELSPECULAIRE_HPP
