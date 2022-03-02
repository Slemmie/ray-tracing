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

int main(int argc, char** argv) {
	
	assert(argc >= 2);
	
	int num_threads = std::stoi(argv[1]);
	
	gp::init();
	
	gp::Single_texture_static_renderer* stst = new gp::Single_texture_static_renderer;
	
	//int width = gp::window_width;
	//int height = gp::window_height;
	
	double aspect_ratio = (double) gp::window_width / (double) gp::window_height;
	int im_w = gp::window_width / 1;
	int im_h = static_cast <int> ((double) im_w / aspect_ratio);
	int samples_per_pixel = 100;
	int max_depth = 10;
	
	Hittable_list world;
	auto material_ground = std::make_shared <Lambertian> (vec3(0.8, 0.8, 0.0));
	auto material_center = std::make_shared <Lambertian> (vec3(0.7, 0.3, 0.3));
	auto material_left = std::make_shared <Metal> (vec3(0.8, 0.8, 0.8));
	auto material_right = std::make_shared <Metal> (vec3(0.8, 0.6, 0.2));
	world.push(std::make_shared <Sphere> (vec3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.push(std::make_shared <Sphere> (vec3(0.0, 0.0, -1.0), 0.5, material_center));
	world.push(std::make_shared <Sphere> (vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.push(std::make_shared <Sphere> (vec3(1.0, 0.0, -1.0), 0.5, material_right));
	
	Camera camera;
	
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
