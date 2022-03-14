// implements BVH.h

#include "BVH.h"

#include <algorithm>

BVH_node::BVH_node(const std::vector <std::shared_ptr <Hittable>>& _surface_source,
size_t _ptr_begin, size_t _ptr_end, double _time_begin, double _time_end) {
	auto surfaces = _surface_source;
	
	int axis = Random::range_int(0, 2);
	auto cmp =
	axis == 0 ?
	m_box_compare <0> :
	axis == 1 ?
	m_box_compare <1> :
	m_box_compare <2> ;
	
	size_t surface_size = _ptr_end - _ptr_begin;
	
	switch (surface_size) {
		case 1:
			m_left = m_right = surfaces[_ptr_begin];
			break;
		case 2:
			if (cmp(surfaces[_ptr_begin], surfaces[_ptr_begin + 1])) {
				m_left = surfaces[_ptr_begin];
				m_right = surfaces[_ptr_begin + 1];
			} else {
				m_left = surfaces[_ptr_begin + 1];
				m_right = surfaces[_ptr_begin];
			}
			break;
		default: {
			std::sort(surfaces.begin() + _ptr_begin, surfaces.begin() + _ptr_end, cmp);
			
			size_t mid = surface_size / 2 + _ptr_begin;
			
			m_left = std::make_shared <BVH_node> (surfaces, _ptr_begin, mid, _time_begin, _time_end);
			m_right = std::make_shared <BVH_node> (surfaces, mid, _ptr_end, _time_begin, _time_end);
			
			break;
		}
	};
	
	AABB left_box, right_box;
	
	if (
	!m_left->bounding_box(_time_begin, _time_end, left_box) ||
	!m_right->bounding_box(_time_begin, _time_end, right_box)) {
		std::cerr << "[error]: ill formed BVH_node construction - no bounding box" << std::endl;
	}
	
	m_box = sorrounding_box(left_box, right_box);
}

bool BVH_node::hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const {
	if (!m_box.hit(ray, t_min, t_max)) {
		return false;
	}
	
	bool hit_left = m_left->hit(ray, t_min, t_max, hit_record);
	bool hit_right = m_right->hit(ray, t_min, hit_left ? hit_record.t : t_max, hit_record);
	
	return hit_left || hit_right;
}
