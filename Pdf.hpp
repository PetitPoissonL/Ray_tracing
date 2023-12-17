//
// Created by LI & WANG on 04/05/2022.
//

#ifndef RAY_TRACING_PDF_HPP
#define RAY_TRACING_PDF_HPP

#include "Vecteur3D.hpp"
#include "ONB.hpp"

// Probability density function
class Pdf {
public:
    virtual ~Pdf() = default;
    virtual double valeur(const Vecteur3D &direction) const = 0;
    virtual Vecteur3D generateur() const = 0;
};

class Cos_pdf : public Pdf {
public:
    Onb uvw;
public:
    explicit Cos_pdf(const Vecteur3D &w) {
        uvw.build_from_w(w);
    }


    double valeur(const Vecteur3D &direction) const override {
        double cos = produitScal(normalisation(direction), uvw.w());
        return (cos <= 0) ? 0 : cos/pi;
    }

    Vecteur3D generateur() const override {
        return uvw.local(random_cosine_direction());
    }
};

// Échantillonner les directions vers un élément frappant, comme la lumière.
class Touche_pdf : public Pdf {
public:
    Point3D o;
    shared_ptr<Touche> ptr;

public:
    Touche_pdf(shared_ptr<Touche> p, const Point3D &origine) : o(origine), ptr(p) {}
    virtual ~Touche_pdf() = default;

    double valeur(const Vecteur3D &direction) const override {
        return ptr->pdf_valeur(o, direction);
    }

    Vecteur3D generateur() const override {
        return ptr->random(o);
    }
};

// Faire une densité de mélange du cosinus et de l'échantillonnage de lumière.
class Melange_pdf : public Pdf {
public:
    shared_ptr<Pdf> p[2];

public:
    Melange_pdf(shared_ptr<Pdf> p0, shared_ptr<Pdf> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    double valeur(const Vecteur3D &direction) const override {
        return 0.5 * p[0]->valeur(direction) + 0.5 * p[1]->valeur(direction);
    }

    Vecteur3D generateur() const override {
        if (random_double() < 0.5)
            return p[0]->generateur();
        else
            return p[1]->generateur();
    }
};


#endif //RAY_TRACING_PDF_HPP
