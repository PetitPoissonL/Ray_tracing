//
// Created by Ningyu & Wenchi on 06/02/2022.
//

#ifndef RAY_TRACING_CAMERA_HPP
#define RAY_TRACING_CAMERA_HPP

#include "Outils.hpp"

class Camera {
private:
    Point3D origin;
    Point3D coinEnBasAGauche;
    Vecteur3D horizontal;
    Vecteur3D vertical;

public:
    Camera(Point3D lookFrom, Point3D lookAt, Vecteur3D vueEnHaut, double champVisionVert, double aspectRatio) { // champ de vision vertical
        auto theta = degresARadians(champVisionVert);
        auto h = tan(theta/2.0);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspectRatio * viewport_height;

        Vecteur3D w = normalisation(lookFrom-lookAt);
        Vecteur3D u = normalisation(produitVect(vueEnHaut, w));
        Vecteur3D v = produitVect(w, u);

        origin = lookFrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        coinEnBasAGauche = origin - horizontal/2 - vertical/2 - w;
    }

    Ray get_ray(double s, double t) const {
        return Ray(origin, coinEnBasAGauche+s*horizontal+t*vertical-origin);
    }
};

#endif //RAY_TRACING_CAMERA_HPP
