//
// Created by Ningyu & Wenchi on 10/03/2022.
//

#ifndef RAY_TRACING_AABB_HPP
#define RAY_TRACING_AABB_HPP

#include "Outils.hpp"

class AABB {
public:
    Point3D minimum;
    Point3D maximum;

public:
    AABB() = default;
    AABB(const Point3D& a, const Point3D& b) { minimum = a; maximum = b;}
    virtual ~AABB() = default;

    Point3D min() const {return minimum; }
    Point3D max() const {return maximum; }

    bool hit(const Ray& r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            double invD = 1.0f / r.direction()[a];
            double t0 = (min()[a] - r.origin()[a]) * invD;
            double t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }
};

AABB surroundingBox(const AABB& box0, const AABB& box1){
    Point3D small(fmin(box0.min().getX(), box1.min().getX()),
                  fmin(box0.min().getY(), box1.min().getY()),
                  fmin(box0.min().getZ(), box1.min().getZ()));

    Point3D big(fmax(box0.max().getX(), box1.max().getX()),
                fmax(box0.max().getY(), box1.max().getY()),
                fmax(box0.max().getZ(), box1.max().getZ()));

    return AABB(small,big);
}



#endif //RAY_TRACING_AABB_HPP
