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

		scattered = ray(rec.p, scatter_dir, r.time());
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
		scattered = ray(rec.p, reflected, r.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0.f);
	}
};

class dielectric : public material {
private:
	float refraction_index;

	static float reflectance(float cos_theta, float _refraction_index) {
		// Schlick's approximation for reflectance
		float r0 = (1.f - _refraction_index) / (1.f + _refraction_index);
		r0 *= r0;
		return r0 + (1.f - r0) * std::powf(1.f - cos_theta, 5);
	}
public:
	dielectric(float _refraction_index) : refraction_index(_refraction_index) {}

	bool scatter(const ray& r, const hit_record& rec, colour& attenuation, ray& scattered) const override {
		attenuation = colour(1.f, 1.f, 1.f);
		float ri = rec.front_face ? (1.f / refraction_index) : refraction_index;

		vec3 unit_dir = unit_vector(r.direction());

		float cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.f);
		float sin_theta = std::sqrtf(1.f - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.f;
		vec3 dir = (cannot_refract || reflectance(cos_theta, ri) > random_float()) ? reflect(unit_dir, rec.normal) : refract(unit_dir, rec.normal, ri);
		scattered = ray(rec.p, dir, r.time());
		return true;
	}
};

#endif