#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
private:
	unsigned int image_height;
	float pixel_samples_scale;  // Color scale factor for a sum of pixel samples 
	point3 center, pixel_upper_left;
	vec3 pixel_du, pixel_dv;
	vec3 u, v, w;  // Camera frame basis vectors

	void initialize() {
		// Calculate Image Height
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.f / samples_per_pixel;

		// Camera
		center = lookfrom;
		float focal_length = (lookfrom - lookat).length();
		float theta = to_radian(vertical_fov);
		float h = std::tanf(theta * 0.5f);
		float viewport_height = 2.f * h * focal_length;
		float viewport_width = viewport_height * (float(image_width) / image_height);

		// Calculate basis vectors
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		
		// Calculate vertical and horizontal viewport edge vectors
		vec3 viewport_u = viewport_width * u;
		vec3 viewport_v = viewport_height * -v;

		// Calculate horizontal and vertical differences
		pixel_du = viewport_u / image_width;
		pixel_dv = viewport_v / image_height;

		auto viewport_upper_left = center - (focal_length * w) - (viewport_u / 2) - (viewport_v / 2);
		pixel_upper_left = viewport_upper_left + 0.5f * (pixel_du + pixel_dv);
	}

	vec3 sample_square() const {
		// Returns the vector to a random point in the [-.5,-.5]-[.5,.5] unit square
		return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.f);
	}

	ray get_ray(int _x, int _y) const {
		// Construct a camera ray originating from the origin and directed at ramdomly
		// sampled point around the pixel location (_x, _y)
		vec3 offset = sample_square();
		point3 pixel_sample = pixel_upper_left + ((_x + offset.x) * pixel_du) + ((_y + offset.y) * pixel_dv);
		point3 ray_orig = center;
		point3 ray_dir = pixel_sample - ray_orig;
		return ray(ray_orig, ray_dir);
	}

	colour ray_colour(const ray& r, unsigned int depth, const hittable& world) const {
		if (depth <= 0) return colour(0.f, 0.f, 0.f);
		
		hit_record rec;
		if (world.hit(r, interval(0.001f, inf), rec)) {
			ray scattered;
			colour attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_colour(scattered, depth - 1, world);
			return colour(0.f, 0.f, 0.f);
		}

		vec3 unit_dir = unit_vector(r.direction());
		float a = 0.5f * (unit_dir.y + 1.f);
		return (1.f - a) * colour(1.f, 1.f, 1.f) + a * colour(0.5f, 0.7f, 1.f);
	}
public:
	// Rendered Image Setup
	float aspect_ratio = 1.f;
	unsigned int image_width = 100;
	unsigned int samples_per_pixel = 10;
	unsigned int max_depth = 10;  // Maximum number of ray bounces into scene

	float vertical_fov = 90.f;  // Vertical field of view
	point3 lookfrom = point3(0.f, 0.f, 0.f);
	point3 lookat = point3(0.f, 0.f, -1.f);
	vec3 vup = vec3(0.f, 1.f, 0.f);

	void render(const hittable& world) {
		initialize();
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (unsigned int _y = 0; _y < image_height; _y++) {
			std::clog << "\rScanlines remaining: " << (image_height - _y) << ' ' << std::flush;
			for (unsigned int _x = 0; _x < image_width; _x++) {
				colour pixel_colour = (0.f, 0.f, 0.f);
				for (unsigned int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(_x, _y);
					pixel_colour += ray_colour(r, max_depth, world);
				}
				write_colour(std::cout, pixel_samples_scale * pixel_colour);
			}
		}
		std::clog << "\rDone.\n";
	}
};

#endif