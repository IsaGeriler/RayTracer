#pragma once
#ifndef AABB_H
#define AABB_H

#include "core.h"

class aabb {
public:
	interval x, y, z;

	aabb() {}
	aabb(const interval& _x, const interval& _y, const interval& _z) : x(_x), y(_y), z(_z) {}

	aabb(const point3& a, const point3& b) {
		x = (a.x <= b.x) ? interval(a.x, b.x) : interval(b.x, a.x);
		y = (a.y <= b.y) ? interval(a.y, b.y) : interval(b.y, a.y);
		z = (a.z <= b.z) ? interval(a.z, b.z) : interval(b.z, a.z);
	}

	aabb(const aabb& box0, const aabb& box1) {
		x = interval(box0.x, box1.x);
		y = interval(box0.y, box1.y);
		z = interval(box0.z, box1.z);
	}

	const interval& axis_interval(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
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

	int longest_axis() const {
		if (x.size() > y.size()) return x.size() > z.size() ? 0 : 2;
		else return y.size() > z.size() ? 1 : 2;
	}

	static const aabb empty, universe;
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif