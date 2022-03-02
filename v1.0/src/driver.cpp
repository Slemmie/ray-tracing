// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"

#include "util/vec3.h"

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
	
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			double r = double(j) / (width - 1);
			double g = double(i) / (height - 1);
			double b = 0.45;
			
			scr[i * width + j] = vec3(255.999 * r, 255.999 * g, 255.999 * b);
		}
	}
	
	stst->update_texture(scr, width, height);
	
	while (!glfwWindowShouldClose(gp::window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		stst->on_update();
		
		glfwSwapBuffers(gp::window);
		
		glfwPollEvents();
	}
	
	delete(stst);
	
	gp::terminate();
	
}
