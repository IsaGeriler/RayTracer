#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "core.h"
#include "hittable.h"

class hittable_list : public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void add(std::shared_ptr<hittable> object) { objects.emplace_back(object); }
	void clear() { objects.clear(); }

	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		hit_record temp_rec;
		bool hit_anything = false;
		float closest_so_far = t_max;

		for (const auto& object : objects) {
			if (object->hit(r, t_min, t_max, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
};

#endif