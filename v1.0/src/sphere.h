// sphere surface

#pragma once

#include "hittable.h"
#include "ray.h"
#include "util/vec3.h"

#include <memory>

class Material;

class Sphere : public Hittable {
	
public:
	
	Sphere() { }
	
	Sphere(const vec3d _center, double _radius, std::shared_ptr <Material> _material) :
	m_center(_center),
	m_radius(_radius),
	m_material(_material)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	inline vec3d& center() { return m_center; }
	
	inline double& radius() { return m_radius; }
	
private:
	
	vec3d m_center;
	
	double m_radius;
	
	std::shared_ptr <Material> m_material;
	
};
