//
// Created by Ningyu & Wenchi on 31/01/2022.
//

#ifndef LANCER_RAYONS_COULEUR_HPP
#define LANCER_RAYONS_COULEUR_HPP
#include "Vecteur3D.hpp"
#include <iostream>

// Écrire la valeur [0,255] de chaque composante de couleur
// Pour l'anti-crénelage, on met plusieurs exemples dans un pixel
Color returnCouleur(Color pixelCouleur, int exemplesParPixel) {
    auto r = pixelCouleur.getX();
    auto g = pixelCouleur.getY();
    auto b = pixelCouleur.getZ();

    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divisez la couleur par le nombre d'échantillons et corrigez le gamma pour un gamma 2
    auto echelle = 1.0 / exemplesParPixel;
    r = sqrt(echelle*r);
    g = sqrt(echelle*g);
    b = sqrt(echelle*b);

    // Écrire chaque composant de couleur dans [0,255]
//    out << static_cast<int>(256 * sandwich(r, 0.0, 0.999)) << ' '
//        << static_cast<int>(256 * sandwich(g, 0.0, 0.999)) << ' '
//        << static_cast<int>(256 * sandwich(b, 0.0, 0.999)) << '\n';
    r = 256 * sandwich(r, 0.0, 0.999);
    g = 256 * sandwich(g, 0.0, 0.999);
    b = 256 * sandwich(b, 0.0, 0.999);

    return {r, g, b};
}

void writeCouleur(std::ostream &out, Color pixelCouleur, int exemplesParPixel){

        auto r = pixelCouleur.getX();
        auto g = pixelCouleur.getY();
        auto b = pixelCouleur.getZ();

    // Divisez la couleur par le nombre d'échantillons et corrigez le gamma pour un gamma 2
    auto echelle = 1.0 / exemplesParPixel;
    r = sqrt(echelle*r);
    g = sqrt(echelle*g);
    b = sqrt(echelle*b);

    // Écrire chaque composant de couleur dans [0,255]
    out << static_cast<int>(256 * sandwich(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * sandwich(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * sandwich(b, 0.0, 0.999)) << '\n';
}

void writeCouleur_out(std::ostream &out, Color pixelCouleur){

    auto r = pixelCouleur.getX();
    auto g = pixelCouleur.getY();
    auto b = pixelCouleur.getZ();

    // Écrire chaque composant de couleur dans [0,255]
    out << static_cast<int>(r) << ' '
        << static_cast<int>(g) << ' '
        << static_cast<int>(b) << '\n';
}

#endif //LANCER_RAYONS_COULEUR_HPP
