//
// Created by 李宁豫 on 11/02/2022.
//

#ifndef RAY_TRACING_MATERIELDIFFUSE_HPP
#define RAY_TRACING_MATERIELDIFFUSE_HPP

#include "Materiel.hpp"
#include "Texture.hpp"

/*
 * Sur les objets de ce matériau, la lumière est réfléchie de manière diffuse.
 * La reflection est dans la direction aléatoire.
 */
class MaterielDiffuse : public Materiel {
public:
    // L'albédo est le pouvoir réfléchissant d'une surface
    // C'est-à-dire le rapport du flux d'énergie lumineuse réfléchie au flux d'énergie lumineuse incidente
    shared_ptr<Texture> albedo;

public:
    explicit MaterielDiffuse(const Color &a) : albedo(make_shared<SolideCouleur>(a)){}
    explicit MaterielDiffuse(shared_ptr<Texture> a) : albedo(a){}
    virtual ~MaterielDiffuse() = default;

    bool diffuser(const Ray &r, const touche_enreg &enreg, diffuse_enreg &dEnreg) const override{
        dEnreg.isSpeculaire = false;
        dEnreg.atenuation = albedo->valeur(enreg.u, enreg.v, enreg.p);
        dEnreg.ptr_pdf = make_shared<Cos_pdf>(enreg.normal);
        return true;
    }
    double diffuser_pdf(const Ray &r, const touche_enreg &enreg, const Ray &diffuse) const override {
        double cos = produitScal(enreg.normal, normalisation(diffuse.direction()));
        return cos < 0 ? 0 : cos / pi;
    }
};

#endif //RAY_TRACING_MATERIELDIFFUSE_HPP
