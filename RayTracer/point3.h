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

inline vec3 unit(const vec3& v) {
	return v / v.length();
}

#endif