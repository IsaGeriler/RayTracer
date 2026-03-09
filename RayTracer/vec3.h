#pragma once

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
public:
	union {
		struct { float x, y, z; };
		float v[3];
	};

	vec3(float _x = 0.f, float _y = 0.f, float _z = 0.f) : x(_x), y(_y), z(_z) {}

	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }

	vec3& operator+=(const vec3& v1) { x += v1.x; y += v1.y; z += v1.z; return *this; }
	vec3 operator+(const vec3& v1) const { return vec3(x + v1.x, y + v1.y, z + v1.z); }
	
	vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }

	vec3& operator/=(float s) { return *this *= 1.f / s; }
	vec3 operator/(float s) const { float invS = 1.f / s; return vec3(x * invS, y * invS, z * invS); }

	vec3 operator-() const { return vec3(-x, -y, -z); }

	float length() const { return legthSquared(); }
	float legthSquared() const { return ((x * x) + (y * y) + (z * z)); }
};

#endif