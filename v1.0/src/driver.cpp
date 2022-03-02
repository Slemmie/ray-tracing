// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"

#include "util/util.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"

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
	int im_w = gp::window_width * 4;
	int im_h = static_cast <int> ((double) im_w / aspect_ratio);
	
	Hittable_list world;
	world.push(std::make_shared <Sphere> (vec3(0.0, 0.0, -1.0), 0.5));
	world.push(std::make_shared <Sphere> (vec3(0.0, -100.5, -1.0), 100.0));
	world.push(std::make_shared <Sphere> (vec3(-2.0, 0.75, -2.0), 0.3));
	
	double vp_h = 2.0;
	double vp_w = aspect_ratio * vp_h;
	double focal_length = 1.0;
	
	vec3 origin = vec3(0.0, 0.0, 0.0);
	vec3 horizontal = vec3(vp_w, 0.0, 0.0);
	vec3 vertical = vec3(0.0, vp_h, 0.0);
	vec3 low_lef_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, focal_length);
	
	vec3d scr[im_w * im_h];
	
	for (int j = im_h - 1; j >= 0; j--) {
		for (int i = 0; i < im_w; i++) {
			double u = double(i) / double(im_w - 1);
			double v = double(j) / double(im_h - 1);
			Ray ray(origin, low_lef_corner + u * horizontal + v * vertical - origin);
			vec3 color = ray_color(ray, world);
			scr[j * im_w + i] = color * 255.999;
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
