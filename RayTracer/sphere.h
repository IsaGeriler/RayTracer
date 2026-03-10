#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "core.h"
#include "hittable.h"

class sphere : public hittable {
private:
	point3 center;
	float radius;
public:
	sphere(const point3& _center, float _radius) : center(_center), radius(std::fmax(0, _radius)) {}
	
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		vec3 oc = center - r.origin();
		float a = dot(r.direction(), r.direction());
		float h = dot(r.direction(), oc);
		float c = oc.legth_squared() - radius * radius;

		float delta = h * h - a * c;
		if (delta < 0.f) return false;

		// Find the nearest root between t_min and t_max
		float root =  (h - std::sqrt(delta)) / a;
		if (root <= t_min || root >= t_max) {
			root = (h + std::sqrt(delta)) / a;
			if (root <= t_min || root >= t_max) return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}
};

#endif