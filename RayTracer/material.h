#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
	virtual ~material() = default;
	virtual bool scatter(const ray& r, const hit_record& rec, colour& attenuation, ray& scattered) const { return false; }
};

class lambertian : public material {
private:
	colour albedo;
public:
	lambertian(const colour& _albedo) : albedo(_albedo) {}

	bool scatter(const ray& r, const hit_record& rec, colour& attenuation, ray& scattered) const override {
		vec3 scatter_dir = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_dir.near_zero()) scatter_dir = rec.normal;

		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
private:
	colour albedo;
	float fuzz;
public:
	metal(const colour& _albedo, float _fuzz) : albedo(_albedo), fuzz(_fuzz < 1.f ? _fuzz : 1.f) {}

	bool scatter(const ray& r, const hit_record& rec, colour& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0.f);
	}
};

#endif