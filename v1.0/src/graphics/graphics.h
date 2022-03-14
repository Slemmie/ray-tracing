// main header for graphics related things

#pragma once

#include <glew/include/glew.h>
#include <glfw/include/glfw3.h>

namespace gp {
	
	// must be called before using any other glfw/glew functionality
	void init();
	
	// should be called only once, and does not allow initializing again
	void terminate();
	
} /// namespace gp
