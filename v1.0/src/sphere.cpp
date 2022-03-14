// implements sphere.h

#include "sphere.h"

bool Sphere::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	vec3 oc = ray.origin() - m_center;
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
	vec3 outward_normal = (hit_record.point - m_center) / m_radius;
	hit_record.set_face_normal(ray, outward_normal);
	hit_record.material = m_material;
	
	return true;
}
