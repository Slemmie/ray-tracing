// bounding volume hierarchy
// data structur for faster queries of first hit() in a scene

#include "util/util.h"

#include "hittable.h"
#include "hittable_list.h"

#include <memory>

class BVH_node : public Hittable {
	
public:
	
	BVH_node() { }
	
	BVH_node(const Hittable_list& _hittable_list, double _time_begin, double _time_end) :
	BVH_node(hittable_list.surfaces(), 0, hittable_list.surfaces().size(), time_begin, time_end)
	{ }
	
	BVH_node(const std::vector <std::shared_ptr <Hittable>>& _surface_source,
	size_t _ptr_begin, size_t _ptr_end, double _time_begin, double _time_end);
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual constexpr bool bounding_box(double time_begin, double time_end, AABB& result_box) const override {
		result_box = m_box;
		return true;
	}
	
private:
	
	std::shared_ptr <Hittable> m_left, m_right;
	
	AABB m_box;
	
};
