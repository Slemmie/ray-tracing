// base class for surfaces

#pragma once

#include "util/vec3.h"
#include "ray.h"

struct Hit_record {
	
	vec3d point;
	
	vec3d normal;
	
	double t;
	
};

class Hittable {
	
public:
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const = 0;
	
};
