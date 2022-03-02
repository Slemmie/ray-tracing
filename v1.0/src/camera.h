// camera class

#pragma once

#include "util/util.h"

namespace gp {
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

class Camera {
	
public:
	
	Camera(vec3d look_from, vec3d look_at, vec3d vup, double _vertical_fov, double _aspect_ratio) {
		auto theta = deg_to_rad(_vertical_fov);
		double h = tan(theta / 2.0);
		double vp_h = 2.0 * h;
		double vp_w = _aspect_ratio * vp_h;
		
		auto w = (look_from - look_at).unit_vector();
		auto u = cross(vup, w).unit_vector();
		auto v = cross(w, u);
		
		m_origin = look_from;
		m_horizontal = vp_w * u;
		m_vertical = vp_h * v;
		m_low_lef_corner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - w;
	}
	
	constexpr Rayd get_ray(double s, double t) const {
		return Ray(m_origin, m_low_lef_corner + s * m_horizontal + t * m_vertical - m_origin);
	}
	
private:
	
	vec3d m_origin;
	vec3d m_horizontal;
	vec3d m_vertical;
	vec3d m_low_lef_corner;
	
};
