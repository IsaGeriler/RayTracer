#include <iostream>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main(int argc, char** argv) {
	// World Setup
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0.f, 0.f, -1.f), 0.5f));
	world.add(std::make_shared<sphere>(point3(0.f, -100.5f, -1.f), 100.f));

	// Camera Setup
	camera cam;
	cam.aspect_ratio = 16.f / 9.f;
	cam.image_width = 1024;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	// Render Scene
	cam.render(world);
}