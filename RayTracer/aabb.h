#pragma once
#ifndef AABB_H
#define AABB_H

#include "core.h"

class aabb {
public:
	interval x, y, z;

	aabb() {}
	aabb(interval _x, interval _y, interval _z) : x(_x), y(_y), z(_z) {}
	aabb(const point3& a, const point3& b) {
		x = (a.x <= b.x) ? interval(a.x, b.x) : interval(b.x, a.x);
		y = (a.y <= b.y) ? interval(a.y, b.y) : interval(b.y, a.y);
		z = (a.z <= b.z) ? interval(a.z, b.z) : interval(b.z, a.z);
	}

	const interval& axis_interval(int n) const {
		if (n == 0) return x;
		if (n == 1) return y;
		if (n == 2) return z;
	}

	bool hit(const ray& r, interval ray_t) const {
		const point3& ray_orig = r.origin();
		const vec3& ray_dir = r.direction();

		for (int axis = 0; axis < 3; axis++) {
			const interval& ax = axis_interval(axis);
			const float inv_ax_dir = 1.f / ray_dir[axis];

			float t0 = (ax.min - ray_orig[axis]) * inv_ax_dir;
			float t1 = (ax.max - ray_orig[axis]) * inv_ax_dir;

			if (t0 < t1) {
				if (t0 > ray_t.min)  ray_t.min = t0;
				if (t1 < ray_t.max)  ray_t.max = t1;
			}
			else {
				if (t1 > ray_t.min)  ray_t.min = t1;
				if (t0 < ray_t.max)  ray_t.max = t0;
			}

			if (ray_t.max <= ray_t.min) return false;
		}
		return true;
	}
};

#endif