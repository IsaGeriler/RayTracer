#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "aabb.h"
#include "core.h"
#include "hittable.h"

class hittable_list : public hittable {
private:
	aabb bbox;
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void add(std::shared_ptr<hittable> object) {
		objects.emplace_back(object);
		bbox = aabb(bbox, object->bounding_box());
	}

	void clear() { objects.clear(); }

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		float closest_so_far = ray_t.max;

		for (const auto& object : objects) {
			if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}

	aabb bounding_box() const { return bbox; }
};

#endif