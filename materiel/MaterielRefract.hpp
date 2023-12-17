//
// Created by Ningyu & Wenchi on 14/02/2022.
//

#ifndef RAY_TRACING_MATERIELREFRACT_HPP
#define RAY_TRACING_MATERIELREFRACT_HPP

#include "Materiel.hpp"

class MaterielRefract : public Materiel {
public:
    double indiceRefract;

public:
    explicit MaterielRefract(double indice_refraction) : indiceRefract(indice_refraction){}
    virtual ~MaterielRefract() = default;

    bool diffuser(const Ray &rIn, const touche_enreg &enreg, diffuse_enreg &dEnreg) const override {
        dEnreg.isSpeculaire = true;
        dEnreg.ptr_pdf = nullptr;
        dEnreg.atenuation = Color(1.0, 1.0, 1.0);
        double refract_ratio = enreg.front_face ? (1.0/indiceRefract) : indiceRefract;

        Vecteur3D directUnit = normalisation(rIn.direction());
        double cosTheta = fmin(produitScal(-directUnit, enreg.normal), 1.0);
        double sinTheta = sqrt(1.0-cosTheta*cosTheta);

        bool pasRefract = refract_ratio*sinTheta >1.0;
        Vecteur3D direction;
        if (pasRefract || reflectance(cosTheta, refract_ratio) > random_double())
            direction = refleter(directUnit, enreg.normal);
        else
            direction = refracter(directUnit, enreg.normal, refract_ratio);

        dEnreg.raySpeculaire = Ray(enreg.p, direction);
        return true;
    }

private:
    static double reflectance(double cosinus, double indiceReflect) {
        double r0 = (1-indiceReflect)/(1+indiceReflect);
        r0 = r0*r0;
        return r0+(1-r0)* pow((1-cosinus), 5);
    }
};

#endif //RAY_TRACING_MATERIELREFRACT_HPP
