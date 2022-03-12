// base class for surfaces

#pragma once

#include "util/vec3.h"
#include "ray.h"

#include <memory>

class Material;

class AABB;

struct Hit_record {
	
	vec3d point;
	
	vec3d normal;
	
	double t;
	
	std::shared_ptr <Material> material;
	
	bool front_face;
	
	// texture coordinates
	double u, v;
	
	// normal points against the ray
	inline void set_face_normal(const Rayd& ray, const vec3d& outward_normal) {
		front_face = dot(ray.direction(), outward_normal) < 0.0;
		normal = front_face ? outward_normal : -outward_normal;
	}
	
};

class Hittable {
	
public:
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const = 0;
	
	// return false if object does not have a bounding box
	virtual bool bounding_box(double time_begin, double time_end, AABB& result_box) const = 0;
	
};
