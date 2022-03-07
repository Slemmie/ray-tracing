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

AABB sorrounding_box(AABB box0, AABB box1) {
	vec3d smaller(
	fmin(box0.minv().x(), box1.minv().x()),
	fmin(box0.minv().y(), box1.minv().y()),
	fmin(box0.minv().z(), box1.minv().z())
	);
	
	vec3d bigger(
	fmax(box0.maxv().x(), box1.maxv().x()),
	fmax(box0.maxv().y(), box1.maxv().y()),
	fmax(box0.maxv().z(), box1.maxv().z())
	);
	
	return AABB(smaller, bigger);
}
