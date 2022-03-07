// implements movable_sphere.h

#include "movable_sphere.h"

#include "AABB.h"

bool Moving_sphere::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	vec3 oc = ray.origin() - center(ray.time());
	auto a = ray.direction().length_squared();
	auto h_b = dot(oc, ray.direction());
	auto c = oc.length_squared() - m_radius * m_radius;
	
	auto discr = h_b * h_b - a * c;
	if (discr < 0.0) {
		return false;
	}
	auto discr_sqrt = sqrt(discr);
	
	auto root = (-h_b - discr_sqrt) / a;
	if (root < t_min || t_max < root) {
		root = (-h_b + discr_sqrt) / a;
		if (root < t_min || t_max < root) {
			return false;
		}
	}
	
	hit_record.t = root;
	hit_record.point = ray.at(root);
	vec3 outward_normal = (hit_record.point - center(ray.time())) / m_radius;
	hit_record.set_face_normal(ray, outward_normal);
	hit_record.material = m_material;
	
	return true;
}

bool Moving_sphere::bounding_box(double time_begin, double time_end, AABB& result_box) const {
	AABB box_begin(
	center(time_begin) - vec3(m_radius, m_radius, m_radius),
	center(time_begin) + vec3(m_radius, m_radius, m_radius));
	
	AABB box_end(
	center(time_end) - vec3(m_radius, m_radius, m_radius),
	center(time_end) + vec3(m_radius, m_radius, m_radius));
	
	result_box = sorrounding_box(box_begin, box_end);
	return true;
}
