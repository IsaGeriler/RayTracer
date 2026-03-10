#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "core.h"
#include "hittable.h"

class sphere : public hittable {
private:
	ray center;
	float radius;
	std::shared_ptr<material> mat;
	aabb bbox;
public:
	// Stationary Sphere
	sphere(const point3& static_center, float _radius, std::shared_ptr<material> _mat) 
		: center(static_center, vec3(0.f, 0.f, 0.f)), radius(std::fmax(0, _radius)), mat(_mat) {}
	
	// Moving Sphere
	sphere(const point3& center_1, const point3& center_2, float _radius, std::shared_ptr<material> _mat)
		: center(center_1, center_2 - center_1), radius(std::fmax(0, _radius)), mat(_mat) {}


	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		point3 current_center = center.at(r.time());
		vec3 oc = current_center - r.origin();
		float a = dot(r.direction(), r.direction());
		float h = dot(r.direction(), oc);
		float c = oc.legth_squared() - radius * radius;

		float delta = h * h - a * c;
		if (delta < 0.f) return false;

		// Find the nearest root between t_min and t_max
		float root =  (h - std::sqrtf(delta)) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + std::sqrtf(delta)) / a;
			if (!ray_t.surrounds(root)) return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - current_center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}

	// aabb bounding_box() const override { return bbox; }
};

#endif