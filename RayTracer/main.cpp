#include <iostream>

#include "colour.h"
#include "point3.h"
#include "ray.h"
#include "vec3.h"

static float hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = center - r.origin();
	float a = dot(r.direction(), r.direction());
	float b = -2.f * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float delta = b * b - 4 * a * c;

	if (delta < 0.f) return -1.f;
	else return (-b - std::sqrt(delta)) / (2.f * a);
}

static colour ray_colour(const ray& r) {
	auto t = hit_sphere(point3(0, 0, -1), 0.5f, r);
	if (t > 0.f) {
		vec3 n = unit(r.at(t) - vec3(0, 0, -1));
		return 0.5f * colour(n.x + 1, n.y + 1, n.z + 1);
	}
	
	vec3 unit_dir = unit(r.direction());
	float a = 0.5f * (unit_dir.y + 1.f);
	return (1.f - a) * colour(1.f, 1.f, 1.f) + a * colour(0.5f, 0.7f, 1.f);
}

int main(int argc, char** argv) {
	// Rendered Image Setup
	float aspect_ratio = 16.f / 9.f;
	unsigned int image_width = 1024;

	// Calculate Image Height
	unsigned int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera
	float focal_length = 1.f;
	float viewport_height = 2.f;
	float viewport_width = viewport_height * (float(image_width) / image_height);
	point3 camera_center = point3(0.f, 0.f, 0.f);

	// Calculate vertical and horizontal viewport edge vectors
	vec3 viewport_u = vec3(viewport_width, 0.f, 0.f);
	vec3 viewport_v = vec3(0.f, -viewport_height, 0.f);

	// Calculate horizontal and vertical differences
	vec3 pixel_du = viewport_u / image_width;
	vec3 pixel_dv = viewport_v / image_height;

	auto viewport_upper_left = camera_center - vec3(0.f, 0.f, focal_length) - viewport_u / 2 - viewport_v / 2;
	auto pixel_upper_left = viewport_upper_left + 0.5f * (pixel_du + pixel_dv);

	// Render PPM
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (unsigned int y = 0; y < image_height; y++) {
		std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;
		for (unsigned int x = 0; x < image_width; x++) {
			auto pixel_center = pixel_upper_left + (x * pixel_du) + (y * pixel_dv);
			auto ray_direction = pixel_center - camera_center;
			
			ray r(camera_center, ray_direction);
			colour pixel_colour = ray_colour(r);
			
			write_colour(std::cout, pixel_colour);
		}
	}
	std::clog << "\rDone.\n";
}