// implements some of hittable.h

#include "hittable.h"

#include "AABB.h"

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
