#include <iostream>

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

static void scene1() {
	// Define Materials
	std::shared_ptr<material> material_ground = std::make_shared<lambertian>(colour(0.8f, 0.8f, 0.f));
	std::shared_ptr<material> material_center = std::make_shared<lambertian>(colour(0.1f, 0.2f, 0.5f));
	std::shared_ptr<material> material_left = std::make_shared<dielectric>(1.5f);
	std::shared_ptr<material> material_bubble = std::make_shared<dielectric>(1.f / 1.5f);
	std::shared_ptr<material> material_right = std::make_shared<metal>(colour(0.8f, 0.6f, 0.2f), 1.f);

	// World Setup
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0.f, -100.5f, -1.f), 100.f, material_ground));
	world.add(std::make_shared<sphere>(point3(0.f, 0.f, -1.2f), 0.5f, material_center));
	world.add(std::make_shared<sphere>(point3(-1.f, 0.f, -1.f), 0.5f, material_left));
	world.add(std::make_shared<sphere>(point3(-1.f, 0.f, -1.f), 0.4f, material_bubble));
	world.add(std::make_shared<sphere>(point3(1.f, 0.f, -1.f), 0.5f, material_right));
    world = hittable_list(std::make_shared<bvh_node>(world));

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

	cam.defocus_angle = 10.f;
	cam.focus_dist = 3.4f;

	// Render Scene
	cam.render(world);
}

static void scene2() {
    hittable_list world;
    auto ground_material = std::make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colour::random_vector() * colour::random_vector();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    // world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                    auto center2 = center + vec3(0, random_float(0, .5), 0);
                    world.add(std::make_shared<sphere>(center, center2, 0.2, sphere_material));

                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colour::random_vector(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(std::make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vertical_fov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

int main(int argc, char** argv) {
	// Select the desired Scene
	// scene1();
	scene2();
}