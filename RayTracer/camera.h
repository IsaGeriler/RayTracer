#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
private:
	unsigned int image_height;
	point3 center, pixel_upper_left;
	vec3 pixel_du, pixel_dv;

	void initialize() {
		// Calculate Image Height
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		// Camera
		center = point3(0.f, 0.f, 0.f);
		float focal_length = 1.f;
		float viewport_height = 2.f;
		float viewport_width = viewport_height * (float(image_width) / image_height);
		
		// Calculate vertical and horizontal viewport edge vectors
		vec3 viewport_u = vec3(viewport_width, 0.f, 0.f);
		vec3 viewport_v = vec3(0.f, -viewport_height, 0.f);

		// Calculate horizontal and vertical differences
		pixel_du = viewport_u / image_width;
		pixel_dv = viewport_v / image_height;

		auto viewport_upper_left = center - vec3(0.f, 0.f, focal_length) - (viewport_u / 2) - (viewport_v / 2);
		pixel_upper_left = viewport_upper_left + 0.5f * (pixel_du + pixel_dv);
	}

	colour ray_colour(const ray& r, const hittable& world) const {
		hit_record rec;
		if (world.hit(r, interval(0.f, inf), rec)) {
			return 0.5f * (rec.normal + colour(1.f, 1.f, 1.f));
		}

		vec3 unit_dir = unit(r.direction());
		float a = 0.5f * (unit_dir.y + 1.f);
		return (1.f - a) * colour(1.f, 1.f, 1.f) + a * colour(0.5f, 0.7f, 1.f);
	}
public:
	// Rendered Image Setup
	float aspect_ratio = 1.f;
	unsigned int image_width = 100;

	void render(const hittable& world) {
		initialize();
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (unsigned int y = 0; y < image_height; y++) {
			std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;
			for (unsigned int x = 0; x < image_width; x++) {
				auto pixel_center = pixel_upper_left + (x * pixel_du) + (y * pixel_dv);
				auto ray_direction = pixel_center - center;
				ray r(center, ray_direction);

				colour pixel_colour = ray_colour(r, world);
				write_colour(std::cout, pixel_colour);
			}
		}
		std::clog << "\rDone.\n";
	}
};

#endif