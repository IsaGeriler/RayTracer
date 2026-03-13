#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#include "hittable.h"
#include "material.h"

#define ENABLE_MT true

class camera {
private:
	unsigned int image_height;
	float pixel_samples_scale;  // Color scale factor for a sum of pixel samples 
	point3 center, pixel_upper_left;
	vec3 pixel_du, pixel_dv;
	vec3 u, v, w;  // Camera frame basis vectors
	vec3 defocus_disk_u, defocus_disk_v;  // Defocus disks

	void initialize() {
		// Calculate Image Height
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.f / samples_per_pixel;

		// Camera
		center = lookfrom;
		float theta = to_radian(vertical_fov);
		float h = std::tanf(theta * 0.5f);
		float viewport_height = 2.f * h * focus_dist;
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

		auto viewport_upper_left = center - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
		pixel_upper_left = viewport_upper_left + 0.5f * (pixel_du + pixel_dv);

		float defocus_radius = focus_dist * std::tanf(to_radian(defocus_angle * 0.5f));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	point3 defocus_disk_sample() const {
		point3 p = random_in_unit_disk();
		return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
	}

	vec3 sample_square() const {
		// Returns the vector to a random point in the [-.5,-.5]-[.5,.5] unit square
		return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.f);
	}

	ray get_ray(int _x, int _y) const {
		// Construct a camera ray originating from the defocus disc and
		// directed at ramdomly sampled point around the pixel location (_x, _y)
		vec3 offset = sample_square();
		point3 pixel_sample = pixel_upper_left + ((_x + offset.x) * pixel_du) + ((_y + offset.y) * pixel_dv);
		point3 ray_orig = (defocus_angle <= 0) ? center : defocus_disk_sample();
		point3 ray_dir = pixel_sample - ray_orig;
		float ray_time = random_float();
		return ray(ray_orig, ray_dir, ray_time);
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

	float defocus_angle = 0.f;
	double focus_dist = 10.f;

	// Mutex, framebuffer and thread vectors
	std::mutex mtx;
	std::vector<colour> frame_buffer;
	std::vector<std::thread> workers;

	void render(const hittable& world) {
		initialize();
		frame_buffer.resize(image_width * image_height);

		// Start timer to calculate render duration
		auto start_time = std::chrono::high_resolution_clock::now();
		#if ENABLE_MT
			// Multithreading
			unsigned int threads = std::thread::hardware_concurrency();
			if (threads == 0) threads = 6;
			unsigned int rows_per_thread = image_height / threads;

			auto render_scanlines = [&](size_t start, size_t end, unsigned int thread_id) {
				for (size_t line = start; line < end; line++) {
					// Using mutex for remaining scanlines (logging reasons only, slows performance by 5-10%)
					{
						std::lock_guard<std::mutex> lock(mtx);
						std::clog << "\r-- Log: Scanlines remaining [Thread #" << thread_id << "]:" << (end - line) << ' ' << "--" << '\n' << std::flush;
					}

					for (unsigned int _x = 0; _x < image_width; _x++) {
						colour pixel_colour(0.f, 0.f, 0.f);
						for (unsigned int sample = 0; sample < samples_per_pixel; sample++) {
							ray r = get_ray(_x, line);
							pixel_colour += ray_colour(r, max_depth, world);
						}
						frame_buffer[line * image_width + _x] = pixel_samples_scale * pixel_colour;
					}
				}
			};

			for (unsigned int t = 0; t < threads; t++) {
				size_t start = t * rows_per_thread;
				size_t end = (t == threads - 1) ? image_height : (start + rows_per_thread);
				workers.emplace_back(render_scanlines, start, end, t);
			}

			for (auto& w : workers) {
				w.join();
			}
		#else
			for (unsigned int _y = 0; _y < image_height; _y++) {
				// Remaining scanlines (logging reasons only)
				std::clog << "\r-- Log: Scanlines remaining:" << image_height - _y << ' ' << "--" << '\n' << std::flush;
				for (unsigned int _x = 0; _x < image_width; _x++) {
					colour pixel_colour(0.f, 0.f, 0.f);
					for (unsigned int sample = 0; sample < samples_per_pixel; sample++) {
						ray r = get_ray(_x, _y);
						pixel_colour += ray_colour(r, max_depth, world);
					}
					frame_buffer[_y * image_width + _x] = pixel_samples_scale * pixel_colour;
				}
			}
		#endif

		// End timer to calculate render duration
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(end_time - start_time).count();
		std::clog << "\rDone. Render Time: " << duration << "s\n";

		// Save the image output (later extend to .png/.jpg with stb_lib.h instead of using .ppm)
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		for (unsigned int _y = 0; _y < image_height; _y++) {
			for (unsigned int _x = 0; _x < image_width; _x++) {
				write_colour(std::cout, frame_buffer[_y * image_width + _x]);
			}
		}
	}
};

#endif