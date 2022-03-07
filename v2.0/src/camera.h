// camera class

#pragma once

#include "util/util.h"
#include "util/random.h"

namespace gp {
	
	extern int window_width;
	extern int window_height;
	
} /// namespace gp

class Camera {
	
public:
	
	Camera(vec3d look_from, vec3d look_at, vec3d vup, double _vertical_fov, double _aspect_ratio,
	double _aperture, double _focus_dist, double _time_begin = 0.0, double _time_end = 0.0) {
		auto theta = deg_to_rad(_vertical_fov);
		double h = tan(theta / 2.0);
		double vp_h = 2.0 * h;
		double vp_w = _aspect_ratio * vp_h;
		
		m_w = (look_from - look_at).unit_vector();
		m_u = cross(vup, m_w).unit_vector();
		m_v = cross(m_w, m_u);
		
		m_origin = look_from;
		m_horizontal = _focus_dist * vp_w * m_u;
		m_vertical = _focus_dist * vp_h * m_v;
		m_low_lef_corner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - _focus_dist * m_w;
		
		m_lens_radius = _aperture / 2.0;
		
		m_time_begin = _time_begin;
		m_time_end = _time_end;
	}
	
	inline Rayd get_ray(double s, double t) const {
		vec3 rd = m_lens_radius * vec3d::random_in_unit_disk();
		vec3 offset = m_u * rd.x() + m_v * rd.y();
		return Ray(
		m_origin + offset,
		m_low_lef_corner + s * m_horizontal + t * m_vertical - m_origin - offset,
		Random::range(m_time_begin, m_time_end));
	}
	
private:
	
	vec3d m_origin;
	vec3d m_horizontal;
	vec3d m_vertical;
	vec3d m_low_lef_corner;
	vec3d m_u, m_v, m_w;
	double m_lens_radius;
	double m_time_begin;
	double m_time_end;
	
};
