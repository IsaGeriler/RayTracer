#pragma once

#ifndef POINT3_H
#define POINT3_H

#include <iostream>

#include "vec3.h"

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.x << ' ' << v.y << ' ' << v.z << '\n';
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(float s, const vec3& v) {
	return vec3(s * v.x, s * v.y, s * v.z);
}

inline vec3 operator/(float s, const vec3& v) {
	return (1.f/s) * v;
}

inline float dot(const vec3& u, const vec3& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_float(-1.f, 1.f), random_float(-1.f, 1.f), 0.f);
		if (p.legth_squared() < 1) return p;
	}
}

inline vec3 random_unit_vector() {
	while (true) {
		vec3 p = vec3::random_vector(-1.f, 1.f);
		float length_squared = p.legth_squared();
		if (1e-38f < length_squared && length_squared < 1.f) return p / sqrtf(length_squared);
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	return (dot(on_unit_sphere, normal) > 0.f) ? on_unit_sphere : -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_eta) {
	// Snell's Law
	float cos_theta = std::fmin(dot(-uv, n), 1.f);
	vec3 r_out_perpendicular = etai_over_eta * (uv + cos_theta * n);
	vec3 r_out_parallel = -std::sqrtf(std::fabs(1.f - r_out_perpendicular.legth_squared())) * n;
	return r_out_perpendicular + r_out_parallel;
}

#endif