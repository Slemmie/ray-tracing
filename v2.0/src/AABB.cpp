// implements AABB.h

#include "AABB.h"

bool AABB::hit(const Rayd& ray, double t_min, double t_max) const {
	for (int i = 0; i < 3; i++) {
		auto inverse_d = 1.0 / ray.direction()[i];
		
		auto t0 = (minv()[i] - ray.origin()[i]) * inverse_d;
		auto t1 = (maxv()[i] - ray.origin()[i]) * inverse_d;
		
		if (inverse_d < 0.0) {
			std::swap(t0, t1);
		}
		
		t_min = fmax(t_min, t0);
		t_max = fmin(t_max, t1);
		
		if (t_max <= t_min) {
			return false;
		}
	}
	
	return true;
}
