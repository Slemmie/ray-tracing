// implements BVH.h

#include "BVH.h"

bool BVH_node hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	if (!m_box.hit(ray, t_min, t_max)) {
		return false;
	}
	
	bool hit_left = left->hit(ray, t_min, t_max, hit_record);
	bool hit_right = right->hit(ray, t_min, hit_left ? hit_record.t : t_max, hit_record);
	
	return hit_left || hit_right;
}
