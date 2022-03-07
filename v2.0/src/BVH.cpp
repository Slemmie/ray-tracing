// implements BVH.h

#include "BVH.h"

#include <algorithm>

BVH_node::BVH_node(const std::vector <std::shared_ptr <Hittable>>& _surface_source,
size_t _ptr_begin, size_t _ptr_end, double _time_begin, double _time_end) {
	auto surfaces = _surface_source;
	
	int axis = Random::range_int(0, 3);
	auto cmp = m_box_compare <axis>;
	
	size_t surface_size = _ptr_begin - _ptr_end;
	
	switch (surface_size) {
		case 1:
			left = right = surfaces[_ptr_begin];
			break;
		case 2:
			if (cmp(surfaces[_ptr_begin], surfaces[_ptr_begin + 1])) {
				left = surfaces[_ptr_begin];
				right = surfaces[_ptr_begin + 1];
			} else {
				left = surfaces[_ptr_begin + 1];
				right = surfaces[_ptr_begin];
			}
			break;
		default: {
			std::sort(surfaces.begin() + _ptr_begin, surfaces.begin() + _ptr_end, cmp);
			
			size_t mid = surface_size / 2 + _ptr_begin;
			
			left = std::make_shared <BVH_node> (surfaces, _ptr_begin, mid, _time_begin, _time_end);
			right = std::make_shared <BVH_node> (surfaces, mid, _ptr_end, _time_begin, _time_end);
			
			break;
		}
	};
	
	AABB left_box, right_box;
	
	if (
	!left->bounding_box(_time_begin, _time_end, box_left) ||
	!right->bounding_box(_time_begin, _time_end, box_right)) {
		std::cerr << "[error]: ill formed BVH_node construction - no bounding box" << std::endl;
	}
	
	m_box = sorrounding_box(box_left, box_right);
}

bool BVH_node hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	if (!m_box.hit(ray, t_min, t_max)) {
		return false;
	}
	
	bool hit_left = left->hit(ray, t_min, t_max, hit_record);
	bool hit_right = right->hit(ray, t_min, hit_left ? hit_record.t : t_max, hit_record);
	
	return hit_left || hit_right;
}
