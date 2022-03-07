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
