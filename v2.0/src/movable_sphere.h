// movable sphere surface

#pragma once

#include "util/util.h"
#include "ray.h"
#include "hittable.h"

class Material;

class AABB;

// linearly movable
class Moving_sphere : public Hittable {
	
public:
	
	Moving_sphere() { }
	
	Moving_sphere(vec3d _center_begin, vec3d _center_end, double _time_begin, double _time_end,
	double _radius, std::shared_ptr <Material> _material) :
	m_center_begin(_center_begin),
	m_center_end(_center_end),
	m_time_begin(_time_begin),
	m_time_end(_time_end),
	m_radius(_radius),
	m_material(_material)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& result_box) const override;
	
	constexpr vec3d center(double time) const {
		return m_center_begin + ((time - m_time_begin) / (m_time_end - m_time_begin)) *
		(m_center_end - m_center_begin);
	}
	
	constexpr double radius() const { return m_radius; }
	
private:
	
	vec3d m_center_begin, m_center_end;
	double m_time_begin, m_time_end;
	double m_radius;
	std::shared_ptr <Material> m_material;
	
};
