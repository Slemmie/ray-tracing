// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"

#include "util/vec3.h"

#include <vector>
#include <algorithm>

namespace gp {
	
	extern GLFWwindow* window;
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

int main() {
	
	gp::init();
	
	gp::Single_texture_static_renderer* stst = new gp::Single_texture_static_renderer;
	
	int width = gp::window_width;
	int height = gp::window_height;
	
	//unsigned char scr[width * height * 4];
	
	//for (int i = height - 1; i >= 0; i--) {
		//for (int j = 0; j < width; j++) {
			//double r = double(j) / (width - 1);
			//double g = double(i) / (height - 1);
			//double b = 0.45;
			
			//scr[((i) * width + j) * 4 + 0] = (int) (255.999 * r);
			//scr[((i) * width + j) * 4 + 1] = (int) (255.999 * g);
			//scr[((i) * width + j) * 4 + 2] = (int) (255.999 * b);
			//scr[((i) * width + j) * 4 + 3] = 255;
		//}
	//}
	
	vec3d scr[width * height];
	
	std::vector <int> pixel_coords;
	
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			//double r = double(j) / (width - 1);
			//double g = double(i) / (height - 1);
			//double b = 0.45;
			
			//scr[i * width + j] = vec3(255.999 * r, 255.999 * g, 255.999 * b);
			scr[i * width + j] = vec3(10.0, 10.0, 10.0);
			
			pixel_coords.push_back(i * width + j);
		}
	}
	
	stst->update_texture(scr, width, height);
	
	std::random_shuffle(pixel_coords.begin(), pixel_coords.end());
	
	while (!glfwWindowShouldClose(gp::window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		// temporarily simulate slow computation of the pixel values
		static int index = 0;
		for (int k = 0; k < 2048 * 8; k++, index = (index + 1) % (int)pixel_coords.size()) {
			int i = pixel_coords[index] / width;
			int j = pixel_coords[index] % width;
			
			double r = double(j) / (width - 1);
			double g = double(i) / (height - 1);
			double b = 0.45;
			
			stst->update_pixel(vec3(255.999 * r, 255.999 * g, 255.999 * b), j, i);
		}
		
		stst->on_update();
		
		glfwSwapBuffers(gp::window);
		
		glfwPollEvents();
	}
	
	delete(stst);
	
	gp::terminate();
	
}
