// implements some central graphics stuff

#include "graphics.h"

#include <iostream>

namespace gp {
	
	GLFWwindow* window;
	
	int window_width;
	int window_height;
	
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
		
		// set opengl version
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		
		// create the main window
		window = glfwCreateWindow(window_width = 1280, window_height = 760, "ray-tracing", nullptr, nullptr);
		if (!window) {
			std::cerr << "[fatal]: failed to create window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		glfwMakeContextCurrent(window);
		
		if (glewInit() != GLEW_OK) {
			std::cerr << "[fatal]: failed to initialize glew" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		// set viewport to match framebuffer size
		int framebuffer_w, framebuffer_h;
		glfwGetFramebufferSize(window, &framebuffer_w, &framebuffer_h);
		glViewport(0, 0, framebuffer_w, framebuffer_h);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// enable vsync by default
		glfwSwapInterval(1);
		
		// glfw callbacks
		glfwSetFramebufferSizeCallback(window, []
		(GLFWwindow* win, int width, int height) -> void {
			glViewport(0, 0, width, height);
			window_width = width;
			window_height = height;
		});
	}
	
	void terminate() {
		// make sure we do not re-terminate
		static bool is_terminated = false;
		if (is_terminated) {
			std::cerr << "[warning]: attempted to re-terminate graphics" << std::endl;
			return;
		}
		is_terminated = true;
		
		if (window) {
			glfwDestroyWindow(window);
		}
		glfwTerminate();
	}
	
} /// namespace gp
