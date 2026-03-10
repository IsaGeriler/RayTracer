#pragma once
#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable {
private:
	std::shared_ptr<hittable> left_child;
	std::shared_ptr<hittable> right_child;
	aabb bbox;
public:
	bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()) {
		// TO:DO
	}

	bvh_node(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end) {
		// TO:DO
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const {
		if (!bbox.hit(r, ray_t)) return false;
		bool hit_left = left_child->hit(r, ray_t, rec);
		bool hit_right = right_child->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);
		return hit_left || hit_right;
	}
};

#endif