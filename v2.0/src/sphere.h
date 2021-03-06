// sphere surface

#pragma once

#include "hittable.h"
#include "ray.h"
#include "util/vec3.h"
#include "util/util.h"

#include <memory>
#include <cmath>

class Material;

class AABB;

class Sphere : public Hittable {
	
public:
	
	Sphere() { }
	
	Sphere(const vec3d _center, double _radius, std::shared_ptr <Material> _material) :
	m_center(_center),
	m_radius(_radius),
	m_material(_material)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& result_box) const override;
	
	inline vec3d& center() { return m_center; }
	
	inline double& radius() { return m_radius; }
	
private:
	
	vec3d m_center;
	
	double m_radius;
	
	std::shared_ptr <Material> m_material;
	
private:
	
	static inline void m_sphere_uv(double& u, double& v, const vec3d& p) {
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + PI;
		
		u = phi / (PI * 2.0);
		v = theta / PI;
	}
	
};
