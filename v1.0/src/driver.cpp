// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"

#include "util/util.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <vector>
#include <algorithm>
#include <iostream>

namespace gp {
	
	extern GLFWwindow* window;
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

vec3d ray_color(const Rayd& ray, const Hittable& world) {
	Hit_record record;
	if (world.hit(ray, 0.0, INF, record)) {
		return 0.5 * (record.normal + vec3(1.0, 1.0, 1.0));
	}
	vec3 unit_dir = ray.direction().unit_vector();
	auto t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	
	gp::init();
	
	gp::Single_texture_static_renderer* stst = new gp::Single_texture_static_renderer;
	
	//int width = gp::window_width;
	//int height = gp::window_height;
	
	double aspect_ratio = (double) gp::window_width / (double) gp::window_height;
	int im_w = gp::window_width / 2;
	int im_h = static_cast <int> ((double) im_w / aspect_ratio);
	int samples_per_pixel = 100;
	
	Hittable_list world;
	world.push(std::make_shared <Sphere> (vec3(0.0, 0.0, -1.0), 0.5));
	world.push(std::make_shared <Sphere> (vec3(0.0, -100.5, -1.0), 100.0));
	world.push(std::make_shared <Sphere> (vec3(-2.0, 0.75, -2.0), 0.3));
	
	Camera camera;
	
	double vp_h = 2.0;
	double vp_w = aspect_ratio * vp_h;
	
	vec3d scr[im_w * im_h];
	
	for (int j = im_h - 1; j >= 0; j--) {
		for (int i = 0; i < im_w; i++) {
			vec3 color = vec3(0.0, 0.0, 0.0);
			for (int k = 0; k < samples_per_pixel; k++) {
				auto u = (i + Random::real()) / (im_w - 1);
				auto v = (j + Random::real()) / (im_h - 1);
				Ray ray = camera.get_ray(u, v);
				color += ray_color(ray, world);
			}
			color /= (double) samples_per_pixel;
			color.r() = clamp(color.r(), 0.0, 0.999);
			color.g() = clamp(color.g(), 0.0, 0.999);
			color.b() = clamp(color.b(), 0.0, 0.999);
			scr[j * im_w + i] = color * 256.0;
		}
	}
	
	std::cerr << "DONE" << std::endl;
	
	stst->update_texture(scr, im_w, im_h);
	
	while (!glfwWindowShouldClose(gp::window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		stst->on_update();
		
		glfwSwapBuffers(gp::window);
		
		glfwPollEvents();
	}
	
	delete(stst);
	
	gp::terminate();
	
}
