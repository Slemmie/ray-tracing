// implements box.h

#include "box.h"

Box::Box(const vec3d& _p0, const vec3d& _p1, std::shared_ptr <Material> _material) :
m_box_min(_p0),
m_box_max(_p1)
{
	m_sides.push(std::make_shared <XY_rect> (_p0.x(), _p1.x(), _p0.y(), _p1.y(), _p1.z(), _material));
	m_sides.push(std::make_shared <XY_rect> (_p0.x(), _p1.x(), _p0.y(), _p1.y(), _p0.z(), _material));
	
	m_sides.push(std::make_shared <XZ_rect> (_p0.x(), _p1.x(), _p0.z(), _p1.z(), _p1.y(), _material));
	m_sides.push(std::make_shared <XZ_rect> (_p0.x(), _p1.x(), _p0.z(), _p1.z(), _p0.y(), _material));
	
	m_sides.push(std::make_shared <YZ_rect> (_p0.y(), _p1.y(), _p0.z(), _p1.z(), _p1.x(), _material));
	m_sides.push(std::make_shared <YZ_rect> (_p0.y(), _p1.y(), _p0.z(), _p1.z(), _p0.x(), _material));
}

bool Box::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	return m_sides.hit(ray, t_min, t_max, hit_record);
}

bool Box::bounding_box(double time_begin, double time_end, AABB& output_box) const {
	output_box = AABB(m_box_min, m_box_max);
	return true;
}
