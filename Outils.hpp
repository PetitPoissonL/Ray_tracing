//
// Created by Ningyu & Wenchi on 05/02/2022.
//

#ifndef LANCER_RAYONS_OUTILS_HPP
#define LANCER_RAYONS_OUTILS_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <random>

//using
using std::shared_ptr;
using std::make_shared;

// constantes
const double infini = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Fonctions utiles
inline double degresARadians(double  degres) {
    return degres*pi/180.0;
}

// Un réel aléatoire dans l'intervalle 0 <= r < 1
inline double random_double() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    static std::mt19937 generator;
    return dist(generator);
}

// Un réel aléatoire dans [min, max]
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

// rendre un int entre [Min, Max]
inline int random_int(int min, int max) {
    return static_cast<int> (random_double(min, max+1));
}

// rendre une valeur entre [Min, Max]
inline double sandwich(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Headers communs
#include "Ray.hpp"
#include "Vecteur3D.hpp"

#endif //LANCER_RAYONS_OUTILS_HPP
