// glfwSwapBuffers but with ensured vsync - sometimes glfwSwapBuffers ignores vsync options (or are overridden)

#pragma once

namespace gp {
	
	// assumes vsync is enabled
	// if not, don't use this...
	void swap_window_buffers();
	
} /// namespace gp
