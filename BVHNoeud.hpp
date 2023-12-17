//
// Created by Ningyu & Wenchi on 10/03/2022.
//

#ifndef RAY_TRACING_BVHNOEUD_HPP
#define RAY_TRACING_BVHNOEUD_HPP

#include "Outils.hpp"
#include "Touche.hpp"
#include "Liste_touche.hpp"
#include <algorithm>

class BVHNoeud : public Touche{
public:
    shared_ptr<Touche> gauche;
    shared_ptr<Touche> droite;
    AABB box;

public:
    BVHNoeud() = default;
    explicit BVHNoeud(const Liste_touche& list) : BVHNoeud(list.objects, 0, list.objects.size()){}
    BVHNoeud(const std::vector<shared_ptr<Touche>>& src_objects, size_t start, size_t end);
    virtual ~BVHNoeud() = default;

    bool touche(const Ray& r, double t_min, double t_max, touche_enreg& rec) const override;
    bool boundingBox(AABB& outputBox) const override;

    static inline bool box_compare(const shared_ptr<Touche>& a, const shared_ptr<Touche>& b, int axis){
        AABB box_a, box_b;
        if (!a->boundingBox(box_a) || !b->boundingBox(box_b))
            std::cerr << "No bounding box in bvh_node constructor.\n";
        return box_a.min().v[axis] < box_b.min().v[axis];
    }

    static bool box_x_compare (const shared_ptr<Touche>& a, const shared_ptr<Touche>& b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare (const shared_ptr<Touche>& a, const shared_ptr<Touche>& b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare (const shared_ptr<Touche>& a, const shared_ptr<Touche>& b) {
        return box_compare(a, b, 2);
    }

};

bool BVHNoeud::boundingBox(AABB &outputBox) const {
    outputBox = box;
    return true;
}

bool BVHNoeud::touche(const Ray &r, double t_min, double t_max, touche_enreg &rec) const {
    if(box.hit(r, t_min, t_max))
        return false;
    bool hitGauche = gauche->touche(r, t_min, t_max, rec);
    bool hitDroite = droite->touche(r, t_min, hitGauche ? rec.t : t_max, rec);

    return hitGauche || hitDroite;
}

BVHNoeud::BVHNoeud(const std::vector<shared_ptr<Touche>>& src_objects, size_t start, size_t end) {
    auto objects = src_objects;

    int axis = random_int(0,2);
    auto comparateur = (axis==0) ? box_x_compare : (axis==1) ? box_y_compare : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        gauche = droite = objects[start];
    } else if (object_span == 2) {
        if (comparateur(objects[start], objects[start+1])) {
            gauche = objects[start];
            droite = objects[start+1];
        } else {
            gauche = objects[start+1];
            droite = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparateur);

        auto mid = start + object_span/2;
        gauche = make_shared<BVHNoeud>(objects, start, mid);
        droite = make_shared<BVHNoeud>(objects, mid, end);
    }

    AABB box_gauche, box_droite;

    if (!gauche->boundingBox (box_gauche) || !droite->boundingBox(box_droite))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surroundingBox(box_gauche, box_droite);
}


#endif //RAY_TRACING_BVHNOEUD_HPP
