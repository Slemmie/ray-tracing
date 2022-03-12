// box object, has 6 rectangles

#include "util/util.h"
#include "aa_rect.h"
#include "hittable_list.h"

class Box : public Hittable {
	
public:
	
	Box() { }
	
	Box(const vec3d& _p0, const vec3d& _p1, std::shared_ptr <Material> _material);
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& output_box) const override;
	
private:
	
	vec3d m_box_min;
	vec3d m_box_max;
	
	Hittable_list m_sides;
	
};
