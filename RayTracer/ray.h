#pragma once

#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "point3.h"

class ray {
private:
	point3 orig;
	vec3 dir;
	float tm;
public:
	ray() {}
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction), tm(0.f) {}
	ray(const point3& origin, const vec3& direction, float _time) : orig(origin), dir(direction), tm(_time) {}

	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }
	float time() const { return tm; }

	// P(t) = A + tb [A: Ray Origin, b: Ray Direction]
	point3 at(float t) const { return orig + t * dir; }
};

#endif