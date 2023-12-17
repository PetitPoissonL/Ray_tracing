//
// Created by Ningyu & Wenchi on 12/03/2022.
//

#ifndef RAY_TRACING_TEXTURE_HPP
#define RAY_TRACING_TEXTURE_HPP

#include "../Outils.hpp"

/*
 * Pour enrichir les détails de la surface de l'objet,
 * nous créons ici une classe pour le chargement et l'échantillonnage de la texture
 */
class Texture {
public:
    virtual Color valeur(double u, double v, const Point3D& p) const = 0;
};

class SolideCouleur : public Texture {
public:
    Color couleurVal;
public:
    SolideCouleur()= default;
    explicit SolideCouleur(Color c) : couleurVal(c) {}
    SolideCouleur(double r, double g, double b) : SolideCouleur(Color(r,g,b)) {}
    virtual ~SolideCouleur() = default;

    Color valeur(double u, double v, const Point3D& p) const override {
        return couleurVal;
    }
};

#endif //RAY_TRACING_TEXTURE_HPP
