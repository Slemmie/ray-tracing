// implements swap_window_buffers.h

#include "swap_window_buffers.h"

#include <glfw/include/glfw3.h>

#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

namespace gp {
	
	extern GLFWwindow* window;
	
	typedef unsigned long long timestamp;
	
	void swap_window_buffers() {
		static double prev_seconds;
		
		// populate prev_seconds on first call
		static bool first_call = true;
		if (first_call) {
			first_call = false;
			glfwSwapBuffers(window);
			prev_seconds = glfwGetTime();
			return;
		}
		
		glfwSwapBuffers(window);
		
		static bool v_sync_working = true;
		
		if (v_sync_working) {
			timestamp diff_micros = static_cast <timestamp> ((glfwGetTime() - prev_seconds) * 1e6);
			// ~166 fps frame
			if (diff_micros < timestamp(100'000)) {
				v_sync_working = false;
				std::cerr << "[warning]: vsync is not working; enabling backup delay" << std::endl;
			}
		}
		
		if (!v_sync_working) {
			// maybe decrease this if target fps > 100
			double cnst = -50;
			
			// use 60 fps target at the moment
			timestamp diff_micros = static_cast <timestamp> ((glfwGetTime() - prev_seconds) * 1e6);
			timestamp sleep_micros = static_cast <timestamp> (std::max(0LL,
			(long long)std::round(cnst + 1e6 / 60.0)) - (long long) diff_micros);
			
			std::this_thread::sleep_for(std::chrono::microseconds(sleep_micros));
		}
	}
	
} /// namespace gp
