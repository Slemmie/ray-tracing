// list of hittables

#pragma once

#include "hittable.h"

#include <vector>
#include <memory>

class AABB;

class Hittable_list : public Hittable {
	
public:
	
	Hittable_list() { }
	
	Hittable_list(std::shared_ptr <Hittable> _surface) {
		push(_surface);
	}
	
	void clear() {
		m_surfaces.clear();
	}
	
	void push(std::shared_ptr <Hittable> surface) {
		m_surfaces.emplace_back(surface);
	}
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& result_box) const override;
	
	inline std::shared_ptr <Hittable>& operator [] (size_t index) {
		return m_surfaces[index];
	}
	
private:
	
	std::vector <std::shared_ptr <Hittable>> m_surfaces;
	
};
