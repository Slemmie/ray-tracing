// implements some of hittable.h

#include "hittable.h"

bool Translate::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	Ray translated_ray(ray.origin() - m_offset, ray.direction(), ray.time());
	
	if (!m_hittable->hit(translated_ray, t_min, t_max, hit_record)) {
		return false;
	}
	
	hit_record.point += m_offset;
	hit_record.set_face_normal(translated_ray, hit_record.normal);
	
	return true;
}

bool Translate::bounding_box(double time_begin, double time_end, AABB& output_box) const {
	if (!m_hittable->bounding_box(time_begin, time_end, output_box)) {
		return false;
	}
	
	output_box = AABB(output_box.minv() + m_offset, output_box.maxv() + m_offset);
	
	return true;
}

Rotate_y::Rotate_y(std::shared_ptr <Hittable> _hittable, double angle) {
	double radians = deg_to_rad(angle);
	m_sin_theta = sin(radians);
	m_cos_theta = cos(radians);
	
	m_has_box = m_hittable->bounding_box(0.0, 1.0, m_bounding_box);
	
	vec3 min(INF, INF, INF);
	vec3 max = -min;
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				double x = m_bounding_box.maxv().x() * i + m_bounding_box.minv().x() * (1 - i);
				double y = m_bounding_box.maxv().y() * j + m_bounding_box.minv().y() * (1 - j);
				double z = m_bounding_box.maxv().z() * k + m_bounding_box.minv().z() * (1 - k);
				
				double nxt_x = m_cos_theta * x + m_sin_theta * z;
				double nxt_z = -m_sin_theta * x + m_cos_theta * z;
				
				vec3 current(nxt_x, y, nxt_z);
				
				for (int l = 0; l < 3; l++) {
					min[l] = fmin(min[l], current[l]);
					max[l] = fmax(max[l], current[l]);
				}
			}
		}
	}
	
	m_bounding_box = AABB(min, max);
}

bool Rotate_y::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	vec3 origin = ray.origin();
	vec3 direction = ray.direction();
	
	origin[0] = m_cos_theta * ray.origin()[0] - m_sin_theta * ray.origin()[2];
	origin[2] = m_sin_theta * ray.origin()[0] + m_cos_theta * ray.origin()[2];
	
	direction[0] = m_cos_theta * ray.direction()[0] - m_sin_theta * ray.direction()[2];
	direction[2] = m_sin_theta * ray.direction()[0] + m_cos_theta * ray.direction()[2];
	
	Ray rotated_ray(origin, direction, ray.time());
	
	if (!m_hittable->hit(rotated_ray, t_min, t_max, hit_record)) {
		return false;
	}
	
	vec3 point = hit_record.point;
	vec3 normal = hit_record.normal;
	
	point[0] = m_cos_theta * hit_record.normal[0] + m_sin_theta * hit_record.normal[2];
	point[2] = -m_sin_theta * hit_record.normal[0] + m_cos_theta * hit_record.normal[2];
	
	normal[0] = m_cos_theta * hit_record.normal[0] + m_sin_theta * hit_record.normal[2];
	normal[2] = -m_sin_theta * hit_record.normal[0] + m_cos_theta * hit_record.normal[2];
	
	hit_record.point = point;
	hit_record.set_face_normal(rotated_ray, normal);
	
	return true;
}
