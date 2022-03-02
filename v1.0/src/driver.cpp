// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"
#include "graphics/swap_window_buffers.h"

#include "util/util.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <atomic>
#include <thread>

namespace gp {
	
	extern GLFWwindow* window;
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

vec3d ray_color(const Rayd& ray, const Hittable& world, int depth) {
	if (depth <= 0) {
		return vec3(0.0, 0.0, 0.0);
	}
	Hit_record record;
	if (world.hit(ray, 0.001, INF, record)) {
		Ray scattered;
		vec3d attenuation;
		if (record.material->scatter(ray, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return vec3(0.0, 0.0, 0.0);
	}
	vec3 unit_dir = ray.direction().unit_vector();
	auto t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

Hittable_list make_scene() {
	Hittable_list world;
	
	auto ground_material = std::make_shared <Lambertian> (vec3(0.5, 0.5, 0.5));
	world.push(std::make_shared <Sphere> (vec3(0.0, -1000.0, 0.0), 1000.0, ground_material));
	
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = Random::real();
			vec3 center(a + 0.9 * Random::real(), 0.2, b + 0.9 * Random::real());
			if ((center - vec3(4.0, 0.2, 0.0)).length() > 0.9) {
				std::shared_ptr <Material> sphere_material;
				if (choose_mat < 0.8) {
					auto albedo = vec3d::random() * vec3d::random();
					sphere_material = std::make_shared <Lambertian> (albedo);
					world.push(std::make_shared <Sphere> (center, 0.2, sphere_material));
				} else if (choose_mat < 0.95) {
					auto albedo = vec3d::random(0.5, 1.0);
					auto fuzz = Random::range(0.0, 0.5);
					sphere_material = std::make_shared <Metal> (albedo, fuzz);
					world.push(std::make_shared <Sphere> (center, 0.2, sphere_material));
				} else {
					sphere_material = std::make_shared <Dielectric> (1.5);
					world.push(std::make_shared <Sphere> (center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared <Dielectric> (1.5);
	world.push(std::make_shared <Sphere> (vec3(0.0, 1.0, 0.0), 1.0, material1));

	auto material2 = std::make_shared <Lambertian> (vec3(0.4, 0.2, 0.1));
	world.push(std::make_shared <Sphere> (vec3(-4.0, 1.0, 0.0), 1.0, material2));

	auto material3 = std::make_shared <Metal> (vec3(0.7, 0.6, 0.5), 0.0);
	world.push(std::make_shared <Sphere> (vec3(4.0, 1.0, 0.0), 1.0, material3));

	return world;
}

int main(int argc, char** argv) {
	
	assert(argc >= 2);
	
	int num_threads = std::stoi(argv[1]);
	
	gp::init();
	
	gp::Single_texture_static_renderer* stst = new gp::Single_texture_static_renderer;
	
	//int width = gp::window_width;
	//int height = gp::window_height;
	
	//double aspect_ratio = (double) gp::window_width / (double) gp::window_height;
	//int im_w = gp::window_width / 2;
	//int im_h = static_cast <int> ((double) im_w / aspect_ratio);
	double aspect_ratio = 3.0 / 2.0;
	int im_w = 1200;
	int im_h = static_cast <int> ((double) im_w / aspect_ratio);
	int samples_per_pixel = 500;
	int max_depth = 50;
	
	Hittable_list world = make_scene();
	//auto material_ground = std::make_shared <Lambertian> (vec3(0.8, 0.8, 0.0));
	//auto material_center = std::make_shared <Lambertian> (vec3(0.1, 0.2, 0.5));
	//auto material_left = std::make_shared <Dielectric> (1.5);
	//auto material_right = std::make_shared <Metal> (vec3(0.8, 0.6, 0.2), 0.0);
	//world.push(std::make_shared <Sphere> (vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
	//world.push(std::make_shared <Sphere> (vec3( 0.0, 0.0, -1.0), 0.5, material_center));
	//world.push(std::make_shared <Sphere> (vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	//world.push(std::make_shared <Sphere> (vec3(-1.0, 0.0, -1.0), -0.45, material_left));
	//world.push(std::make_shared <Sphere> (vec3( 1.0, 0.0, -1.0), 0.5, material_right));
	
	vec3 look_from = vec3(13.0, 2.0, 3.0);
	vec3 look_at = vec3(0.0, 0.0, 0.0);
	vec3 vup = vec3(0.0, 1.0, 0.0);
	double dist_to_focus = 10.0;
	auto aperture = 0.1;
	Camera camera(look_from, look_at, vup, 20.0, aspect_ratio, aperture, dist_to_focus);
	
	//double vp_h = 2.0;
	//double vp_w = aspect_ratio * vp_h;
	
	vec3d scr[im_w * im_h];
	
	// set the base color and dimensions before launching any threads
	for (int j = im_h - 1; j >= 0; j--) {
		for (int i = 0; i < im_w; i++) {
			scr[j * im_w + i] = vec3(5.0, 5.0, 5.0);
		}
	}
	
	stst->update_texture(scr, im_w, im_h);
	
	// multithreaded access to global index counter
	std::atomic <int> current_index = 0;
	std::atomic <bool> terminate_threads = false;
	
	// a calculating thread runs this
	auto inl_job = [&] () -> void {
		// run until all pixels are filled
		while (current_index < im_h * im_w && !terminate_threads) {
			int index = current_index++;
			if (index >= im_h * im_w) {
				break;
			}
			
			int j = index / im_w;
			int i = index % im_w;
			
			vec3 color = vec3(0.0, 0.0, 0.0);
			for (int k = 0; k < samples_per_pixel; k++) {
				auto u = (i + Random::real()) / (im_w - 1);
				auto v = (j + Random::real()) / (im_h - 1);
				Ray ray = camera.get_ray(u, v);
				color += ray_color(ray, world, max_depth);
			}
			double scale = 1.0 / (double) samples_per_pixel;
			color.r() = sqrt(scale * color.r());
			color.g() = sqrt(scale * color.g());
			color.b() = sqrt(scale * color.b());
			color.r() = clamp(color.r(), 0.0, 0.999);
			color.g() = clamp(color.g(), 0.0, 0.999);
			color.b() = clamp(color.b(), 0.0, 0.999);
			
			stst->update_pixel(color * 256.0, i, j);
		}
	};
	
	// launch threads
	std::vector <std::thread> threads;
	for (int i = 0; i < num_threads; i++) {
		threads.emplace_back(std::thread(inl_job));
	}
	
	while (!glfwWindowShouldClose(gp::window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		stst->on_update();
		
		gp::swap_window_buffers();
		
		glfwPollEvents();
	}
	
	// make sure threads are joined
	for (std::thread& thread : threads) {
		terminate_threads = true;
		thread.join();
	}
	
	delete(stst);
	
	gp::terminate();
	
}
