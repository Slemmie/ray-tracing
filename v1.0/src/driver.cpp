// main driver program
// contains the global entry point

#include "graphics/graphics.h"
#include "graphics/render_image.h"

namespace gp {
	
	extern GLFWwindow* window;
	
} /// namespace gp

int main() {
	
	gp::init();
	
	gp::Single_texture_static_renderer* stst = new gp::Single_texture_static_renderer;
	
	unsigned char c[6 * 4] = {
		255, 255, 255, 255,  255, 255, 128, 255,  0, 0, 255, 255,
		128, 255, 255, 255,  255, 128, 128, 255,  0, 255, 255, 255
	};
	stst->update_texture(c, 3, 2);
	
	while (!glfwWindowShouldClose(gp::window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		stst->on_update();
		
		glfwSwapBuffers(gp::window);
		
		glfwPollEvents();
	}
	
	delete(stst);
	
	gp::terminate();
	
}
