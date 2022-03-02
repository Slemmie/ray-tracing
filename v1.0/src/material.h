// base material class

#pragma once

#include "util/vec3.h"

#include "ray.h"

struct Hit_record;

class Material {
	
public:
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const = 0;
	
};

class Lambertian : public Material {
	
public:
	
	Lambertian(const vec3d& _albedo) :
	m_albedo(_albedo)
	{ }
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const override {
		auto scatter_direction = hit_record.normal + vec3d::random_unit_vec3();
		
		if (scatter_direction.near_zero()) {
			scatter_direction = hit_record.normal;
		}
		
		scattered = Ray(hit_record.point, scatter_direction);
		attenuation = m_albedo;
		return true;
	}
	
private:
	
	vec3d m_albedo;
	
};

class Metal : public Material {
	
public:
	
	Metal(const vec3d& _albedo, double _fuzz) :
	m_albedo(_albedo),
	m_fuzz(_fuzz < 1 ? _fuzz : 1)
	{ }
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const override {
		vec3 reflected = vec3d::reflect((ray.direction()).unit_vector(), hit_record.normal);
		scattered = Ray(hit_record.point, reflected + m_fuzz * vec3d::random_in_unit_sphere());
		attenuation = m_albedo;
		return dot(scattered.direction(), hit_record.normal) > 0.0;
	}
	
private:
	
	vec3d m_albedo;
	
	double m_fuzz;
	
};
