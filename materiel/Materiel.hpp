//
// Created by Ningyu & Wenchi on 10/02/2022.
//

#ifndef RAY_TRACING_MATERIEL_HPP
#define RAY_TRACING_MATERIEL_HPP

#include "../Outils.hpp"
#include "../Touche.hpp"
#include "Texture.hpp"
#include "../ONB.hpp"
#include "../Pdf.hpp"

struct touche_enreg;

struct diffuse_enreg {
    Ray raySpeculaire;
    bool isSpeculaire;
    Color atenuation;
    shared_ptr<Pdf> ptr_pdf;
};

// Comment il v1 a des materiels différents, on crée une classe abstraite Materiel
class Materiel {
public:
    virtual bool diffuser(const Ray &r, const touche_enreg &enreg, diffuse_enreg &dEnreg) const {
        return false;
    }

    virtual Color emis(const Ray &r_in, const touche_enreg & enreg, double u, double v, const Point3D& p) const {
        return {0, 0, 0};
    }

    virtual double diffuser_pdf(const Ray &r, const touche_enreg &enreg, const Ray &diffuse) const {
        return 0;
    }
};

class LumiereDiffuse : public Materiel {
public:
    shared_ptr<Texture> emise;
public:
    explicit LumiereDiffuse(shared_ptr<Texture> a) : emise(a){}
    explicit LumiereDiffuse(Color c) : emise(make_shared<SolideCouleur>(c)) {}

    virtual ~LumiereDiffuse() = default;

    bool diffuser(const Ray &r, const touche_enreg &enreg, diffuse_enreg &dEnreg) const override{
        return false;
    }

    Color emis(const Ray &r_in, const touche_enreg & enreg, double u, double v, const Point3D& p) const override{
        if (enreg.front_face)
            return emise->valeur(u, v, p);
        else
            return {0,0,0,};
    }
};


#endif //RAY_TRACING_MATERIEL_HPP
