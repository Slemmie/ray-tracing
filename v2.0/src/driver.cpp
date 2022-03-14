// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"
#include "graphics/swap_window_buffers.h"

#include "util/util.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "movable_sphere.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "scene.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <string>

// hard coded scene ids for now //

// can be changed from command line

#define SCENE_TWO_SPHERES 1
#define SCENE_TWO_PERLIN_SPHERES 2
#define SCENE_RANDOM_DEMO 3
#define SCENE_EARTH 4
#define SCENE_SIMPLE_LIGHT 5
#define SCENE_CORNELL_BOX 6
#define SCENE_CORNELL_SMOKE 7

int SCENE = SCENE_CORNELL_SMOKE;

//////////////////////////////////

int num_threads = 1;

namespace gp {
	
	extern GLFWwindow* window;
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

vec3d ray_color(const Rayd& ray, const vec3d& background, const Hittable& world, int depth) {
	if (depth <= 0) {
		return vec3(0.0, 0.0, 0.0);
	}
	
	Hit_record record;
	
	if (!world.hit(ray, 0.001, INF, record)) {
		return background;
	}
	
	Ray scattered;
	vec3d attenuation;
	vec3d emitted = record.material->emitted(record.u, record.v, record.point);
	
	if (!record.material->scatter(ray, record, attenuation, scattered)) {
		return emitted;
	}
	
	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

void handle_cl_args(int argc, char** argv) {
	bool arg_found_scene = false;
	bool arg_found_tc = false;
	
	for (int i = 1; i < argc; i++) {
		std::string str = std::string(argv[i]);
		
		if (str.substr(0, 2) == "s=") {
			str = str.substr(2);
			std::cerr << "[info]: setting scene to '" << str << "'" << std::endl;
			if (str == scene::Two_spheres().to_string()) {
				SCENE = SCENE_TWO_SPHERES;
			} else if (str == scene::Two_perlin_spheres().to_string()) {
				SCENE = SCENE_TWO_PERLIN_SPHERES;
			} else if (str == scene::Random_demo().to_string()) {
				SCENE = SCENE_RANDOM_DEMO;
			} else if (str == scene::Earth().to_string()) {
				SCENE = SCENE_EARTH;
			} else if (str == scene::Simple_light().to_string()) {
				SCENE = SCENE_SIMPLE_LIGHT;
			} else if (str == scene::Cornell_box().to_string()) {
				SCENE = SCENE_CORNELL_BOX;
			} else if (str == scene::Cornell_smoke().to_string()) {
				SCENE = SCENE_CORNELL_SMOKE;
			} else {
				std::cerr << "[warning]: unknown scene name provided" << std::endl;
				continue;
			}
			arg_found_scene = true;
			continue;
		}
		
		std::cerr << "[info]: setting thread count to " << argv[i] << std::endl;
		num_threads = std::stoi(argv[i]);
		arg_found_tc = true;
	}
	
	if (!arg_found_scene) {
		static std::string sc_to_s[] = {
			"[unknown]",
			scene::Two_spheres().to_string(),        // SCENE_TWO_SPHERES
			scene::Two_perlin_spheres().to_string(), // SCENE_TWO_PERLIN_SPHERES
			scene::Random_demo().to_string(),        // SCENE_RANDOM_DEMO
			scene::Earth().to_string(),              // SCENE_EARTH
			scene::Simple_light().to_string(),       // SCENE_SIMPLE_LIGHT
			scene::Cornell_box().to_string(),        // SCENE_CORNELL_BOX
			scene::Cornell_smoke().to_string()       // SCENE_CORNELL_BOX
		};
		std::cerr << "[info]: setting scene to '" << sc_to_s[SCENE] << "'" << std::endl;
	}
	
	if (!arg_found_tc) {
		std::cerr << "[info]: setting thread count to " << num_threads << std::endl;
	}
}

int main(int argc, char** argv) {
	
	handle_cl_args(argc, argv);
	
	gp::init();
	
	gp::Single_texture_static_renderer* stst = new gp::Single_texture_static_renderer;
	
	//int width = gp::window_width;
	//int height = gp::window_height;
	
	double aspect_ratio = 16.0 / 9.0;
	int im_w = 400;
	int samples_per_pixel = 100;
	int max_depth = 50;
	
	Hittable_list world;
	vec3 look_from, look_at;
	double vfov = 40.0;
	double aperture = 0.0;
	vec3d background(0.0, 0.0, 0.0);
	
	switch (SCENE) {
		case SCENE_TWO_SPHERES: {
			scene::Two_spheres scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			break;
		}
		case SCENE_TWO_PERLIN_SPHERES: {
			scene::Two_perlin_spheres scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			break;
		}
		case SCENE_RANDOM_DEMO: {
			scene::Random_demo scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			break;
		}
		case SCENE_EARTH: {
			scene::Earth scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			break;
		}
		case SCENE_SIMPLE_LIGHT: {
			scene::Simple_light scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			samples_per_pixel = 400;
			break;
		}
		case SCENE_CORNELL_BOX: {
			scene::Cornell_box scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			samples_per_pixel = 200;
			aspect_ratio = 1.0;
			im_w = 600;
			break;
		}
		case SCENE_CORNELL_SMOKE: {
			scene::Cornell_smoke scen;
			world = scen.get_world();
			scen.set_params(look_from, look_at, vfov, aperture, background);
			samples_per_pixel = 200;
			aspect_ratio = 1.0;
			im_w = 600;
			break;
		}
		default:
			std::cerr << "[fatal]: unknown scene id" << std::endl;
			exit(EXIT_FAILURE);
	};
	
	int im_h = static_cast <int> ((double) im_w / aspect_ratio);
	
	vec3 vup = vec3(0.0, 1.0, 0.0);
	double dist_to_focus = 10.0;
	Camera camera(look_from, look_at, vup, vfov,
	aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
	
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
	std::atomic <int> finished_workers = 0;
	
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
				color += ray_color(ray, background, world, max_depth);
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
		
		finished_workers++;
	};
	
	// launch threads
	std::vector <std::thread> threads;
	for (int i = 0; i < num_threads; i++) {
		threads.emplace_back(std::thread(inl_job));
	}
	
	while (!glfwWindowShouldClose(gp::window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		static bool did_final_flush = false;
		if (!did_final_flush && finished_workers == num_threads) {
			did_final_flush = true;
			stst->flush_pixel_buffer();
		}
		
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
