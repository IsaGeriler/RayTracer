#include <iostream>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main(int argc, char** argv) {
	// Define Materials
	std::shared_ptr<material> material_ground = std::make_shared<lambertian>(colour(0.8f, 0.8f, 0.f));
	std::shared_ptr<material> material_center = std::make_shared<lambertian>(colour(0.1f, 0.2f, 0.5f));
	std::shared_ptr<material> material_left = std::make_shared<dialectric>(1.5f);
	std::shared_ptr<material> material_bubble = std::make_shared<dialectric>(1.f / 1.5f);
	std::shared_ptr<material> material_right = std::make_shared<metal>(colour(0.8f, 0.6f, 0.2f), 1.f);

	// World Setup
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0.f, -100.5f, -1.f), 100.f, material_ground));
	world.add(std::make_shared<sphere>(point3(0.f, 0.f, -1.2f), 0.5f, material_center));
	world.add(std::make_shared<sphere>(point3(-1.f, 0.f, -1.f), 0.5f, material_left));
	world.add(std::make_shared<sphere>(point3(-1.f, 0.f, -1.f), 0.4f, material_bubble));
	world.add(std::make_shared<sphere>(point3(1.f, 0.f, -1.f), 0.5f, material_right));
	
	// Camera Setup
	camera cam;
	cam.aspect_ratio = 16.f / 9.f;
	cam.image_width = 1024;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.vertical_fov = 20.f;
	cam.lookfrom = point3(-2.f, 2.f, 1.f);
	cam.lookat = point3(0.f, 0.f, -1.f);
	cam.vup = vec3(0.f, 1.f, 0.f);

	// Render Scene
	cam.render(world);
}