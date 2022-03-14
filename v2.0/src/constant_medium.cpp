// implements constant_medium.h

#include "constant_medium.h"

bool Constant_medium::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	Hit_record record[2];
	
	if (!m_boundary->hit(ray, -INF, INF, record[0])) {
		return false;
	}
	
	if (!m_boundary->hit(ray, record[0].t + 0.0001, INF, record[1])) {
		return false;
	}
	
	record[0].t = std::max(record[0].t, t_min);
	record[1].t = std::min(record[1].t, t_max);
	
	if (record[0].t >= record[1].t) {
		return false;
	}
	
	record[0].t = std::max(record[0].t, 0.0);
	
	const auto ray_length = ray.direction().length();
	const auto distance_inside_boundary = ray_length * (record[1].t - record[0].t);
	const auto hit_distance = m_neg_inv_density * log(Random::real());
	
	if (hit_distance > distance_inside_boundary) {
		return false;
	}
	
	hit_record.t = record[0].t + hit_distance / ray_length;
	hit_record.point = ray.at(hit_record.t);
	
	// arbitrary
	hit_record.normal = vec3(1.0, 0.0, 0.0);
	hit_record.front_face = true;
	
	hit_record.material = m_phase_function;
	
	return true;
}
