// implements hittable_list.h

#include "hittable_list.h"

bool Hittable_list::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	Hit_record record;
	bool any_hits = false;
	auto closest = t_max;
	
	for (const auto& surface : m_surfaces) {
		if (surface->hit(ray, t_min, closest, record)) {
			any_hits = true;
			closest = record.t;
			hit_record = record;
		}
	}
	
	return any_hits;
}

bool Hittable_list::bounding_box(double time_begin, double time_end, AABB& result_box) const {
	if (m_surfaces.empty()) {
		return false;
	}
	
	AABB passer;
	bool is_first_box = true;
	
	for (const auto& surface : m_surfaces) {
		if (!surface->bounding_box(time_begin, time_end, passer)) {
			return false;
		}
		result_box = is_first_box ? passer : sorrounding_box(result_box, passer);
		is_first_box = false;
	}
	
	return true;
}
