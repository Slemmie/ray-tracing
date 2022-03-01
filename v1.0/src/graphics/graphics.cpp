// implements some central graphics stuff

#include "graphics.h"

#include <iostream>

namespace gp {
	
	void init() {
		// make sure we do not re-init
		static bool is_initialized = false;
		if (is_initialized) {
			std::cerr << "[warning]: attempted to re-init graphics" << std::endl;
			return;
		}
		is_initialized = true;
		
		if (!glfwInit()) {
			std::cerr << "[fatal]: failed to initialize glfw" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		glfwSetErrorCallback([] (int id, const char* message) -> void {
			std::cerr << "[glfw error]: " << message << " (error ID: " << id << ")" << std::endl;
		});
	}
	
} /// namespace gp
