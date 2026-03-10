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

inline vec3 random_unit_vector() {
	while (true) {
		vec3 p = vec3::random_vector(-1.f, 1.f);
		float length_squared = p.legth_squared();
		if (1e-38 < length_squared && length_squared < 1.f) return p / sqrtf(length_squared);
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	return (dot(on_unit_sphere, normal) > 0.f) ? on_unit_sphere : -on_unit_sphere;
}

#endif