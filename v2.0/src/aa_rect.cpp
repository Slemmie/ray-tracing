// implements aa_rect.h

#include "aa_rect.h"

#include "AABB.h"

bool XY_rect::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	double t = (m_k - ray.origin().z()) / ray.direction().z();
	
	if (t < t_min || t > t_max) {
		return false;
	}
	
	double x = ray.origin().x() + t * ray.direction().x();
	double y = ray.origin().y() + t * ray.direction().y();
	
	if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1) {
		return false;
	}
	
	hit_record.u = (x - m_x0) / (m_x1 - m_x0);
	hit_record.v = (y - m_y0) / (m_y1 - m_y0);
	hit_record.t = t;
	
	vec3 outward_normal = vec3(0.0, 0.0, 1.0);
	
	hit_record.set_face_normal(ray, outward_normal);
	hit_record.material = m_material;
	hit_record.point = ray.at(t);
	
	return true;
}

bool XY_rect::bounding_box(double time_begin, double time_end, AABB& output_box) const {
	output_box = AABB(vec3(m_x0, m_y0, m_k - 0.0001), vec3(m_x1, m_y1, m_k + 0.0001));
	return true;
}

bool XZ_rect::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	auto t = (m_k - ray.origin().y()) / ray.direction().y();
	
	if (t < t_min || t > t_max) {
		return false;
	}
	
	double x = ray.origin().x() + t * ray.direction().x();
	double z = ray.origin().z() + t * ray.direction().z();
	
	if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1) {
		return false;
	}
	
	hit_record.u = (x - m_x0) / (m_x1 - m_x0);
	hit_record.v = (z - m_z0) / (m_z1 - m_z0);
	hit_record.t = t;
	
	vec3 outward_normal = vec3(0.0, 1.0, 0.0);
	
	hit_record.set_face_normal(ray, outward_normal);
	hit_record.material = m_material;
	hit_record.point = ray.at(t);
	
	return true;
}

bool XZ_rect::bounding_box(double time_begin, double time_end, AABB& output_box) const {
	output_box = AABB(vec3(m_x0, m_k - 0.0001, m_z0), vec3(m_x1, m_k + 0.0001, m_z1));
	return true;
}

bool YZ_rect::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	double t = (m_k - ray.origin().x()) / ray.direction().x();
	
	if (t < t_min || t > t_max) {
		return false;
	}
	
	double y = ray.origin().y() + t * ray.direction().y();
	double z = ray.origin().z() + t * ray.direction().z();
	
	if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1) {
		return false;
	}
	
	hit_record.u = (y - m_y0) / (m_y1 - m_y0);
	hit_record.v = (z - m_z0) / (m_z1 - m_z0);
	hit_record.t = t;
	
	vec3 outward_normal = vec3(1.0, 0.0, 0.0);
	
	hit_record.set_face_normal(ray, outward_normal);
	hit_record.material = m_material;
	hit_record.point = ray.at(t);
	
	return true;
}

bool YZ_rect::bounding_box(double time_begin, double time_end, AABB& output_box) const {
	output_box = AABB(vec3(m_k - 0.0001, m_y0, m_z0), vec3(m_k + 0.0001, m_y1, m_z1));
	return true;
}
