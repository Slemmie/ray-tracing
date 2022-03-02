// camera class

#pragma once

#include "util/util.h"

namespace gp {
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

class Camera {
	
public:
	
	Camera() {
		double aspect_ratio = (double) gp::window_width / (double) gp::window_height;
		double vp_h = 2.0;
		double vp_w = aspect_ratio * vp_h;
		double focal_length = 1.0;
		
		m_origin = vec3(0.0, 0.0, 0.0);
		m_horizontal = vec3(vp_w, 0.0, 0.0);
		m_vertical = vec3(0.0, vp_h, 0.0);
		m_low_lef_corner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - vec3(0.0, 0.0, focal_length);
	}
	
	constexpr Rayd get_ray(double u, double v) const {
		return Ray(m_origin, m_low_lef_corner + u * m_horizontal + v * m_vertical - m_origin);
	}
	
private:
	
	vec3d m_origin;
	vec3d m_horizontal;
	vec3d m_vertical;
	vec3d m_low_lef_corner;
	
};
