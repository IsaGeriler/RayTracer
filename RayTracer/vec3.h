#pragma once

#ifndef VEC3_H
#define VEC3_H

#include "core.h"

class vec3 {
public:
	union {
		struct { float x, y, z; };
		float v[3];
	};

	vec3(float _x = 0.f, float _y = 0.f, float _z = 0.f) : x(_x), y(_y), z(_z) {}

	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }

	vec3& operator+=(const vec3& v1) {
		x += v1.x; y += v1.y; z += v1.z;
		return *this;
	}

	vec3& operator-=(const vec3& v1) {
		x -= v1.x; y -= v1.y; z -= v1.z;
		return *this;
	}

	vec3& operator*=(float s) {
		x *= s; y *= s; z *= s;
		return *this;
	}

	vec3& operator/=(float s) { return *this *= 1.f / s; }

	vec3 operator/(float s) const {
		float invS = 1.f / s;
		return vec3(x * invS, y * invS, z * invS);
	}

	vec3 operator-() const { return vec3(-x, -y, -z); }

	float length() const { return sqrtf(legth_squared()); }
	float legth_squared() const { return ((x * x) + (y * y) + (z * z)); }

	bool near_zero() const { return (std::fabs(x) < 1e-8f) && (std::fabs(y) < 1e-8f) && (std::fabs(z) < 1e-8f); }

	static vec3 random_vector() { return vec3(random_float(), random_float(), random_float()); }
	static vec3 random_vector(float min, float max) { return vec3(random_float(min, max), random_float(min, max), random_float(min, max)); }
};

#endif