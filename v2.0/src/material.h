// base material class

#pragma once

#include "util/vec3.h"

#include "ray.h"
#include "texture.h"

#include <memory>

struct Hit_record;

class Material {
	
public:
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const = 0;
	
	virtual vec3d emitted(double u, double v, const vec3d& p) const {
		return vec3(0.0, 0.0, 0.0);
	}
	
};

class Lambertian : public Material {
	
public:
	
	Lambertian(const vec3d& _albedo) :
	m_albedo(std::make_shared <tex::Solid_color> (_albedo))
	{ }
	
	Lambertian(std::shared_ptr <tex::Texture> _texture) :
	m_albedo(_texture)
	{ }
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const override {
		auto scatter_direction = hit_record.normal + vec3d::random_unit_vec3();
		
		if (scatter_direction.near_zero()) {
			scatter_direction = hit_record.normal;
		}
		
		scattered = Ray(hit_record.point, scatter_direction, ray.time());
		attenuation = m_albedo->at(hit_record.u, hit_record.v, hit_record.point);
		return true;
	}
	
private:
	
	std::shared_ptr <tex::Texture> m_albedo;
	
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
		scattered =
		Ray(hit_record.point,
		reflected + m_fuzz * vec3d::random_in_unit_sphere(),
		ray.time());
		attenuation = m_albedo;
		return dot(scattered.direction(), hit_record.normal) > 0.0;
	}
	
private:
	
	vec3d m_albedo;
	
	double m_fuzz;
	
};

class Dielectric : public Material {
	
public:
	
	Dielectric(double _refraction_index) :
	m_refraction_index(_refraction_index)
	{ }
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const override {
		attenuation = vec3(1.0, 1.0, 1.0);
		double refraction_ratio = hit_record.front_face ? (1.0 / m_refraction_index) : m_refraction_index;
		vec3 unit_direction = ray.direction().unit_vector();
		double cos_theta = fmin(dot(-unit_direction, hit_record.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || m_reflectance(cos_theta, refraction_ratio) > Random::real()) {
			direction = vec3d::reflect(unit_direction, hit_record.normal);
		} else {
			direction = vec3d::refract(unit_direction, hit_record.normal, refraction_ratio);
		}
		scattered = Ray(hit_record.point, direction, ray.time());
		return true;
	}
	
private:
	
	double m_refraction_index;
	
	static double m_reflectance(double cos, double ref) {
		auto r0 = (1.0 - ref) / (1.0 + ref);
		r0 *= r0;
		return r0 + (1.0 - r0) * pow((1.0 - cos), 5.0);
	}
	
};

class Diffuse_light : public Material {
	
public:
	
	Diffuse_light(std::shared_ptr <tex::Texture> _texture) :
	m_emit(_texture)
	{ }
	
	Diffuse_light(const vec3d& _color) :
	m_emit(std::make_shared <tex::Solid_color> (_color))
	{ }
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const override {
		return false;
	}
	
	virtual vec3d emitted(double u, double v, const vec3d& p) const override {
		return m_emit->at(u, v, p);
	}
	
private:
	
	std::shared_ptr <tex::Texture> m_emit;
	
};

class Isotropic : public Material {
	
public:
	
	Isotropic(const vec3d& _color) :
	m_albedo(std::make_shared <tex::Solid_color> (_color))
	{ }
	
	Isotropic(std::shared_ptr <tex::Texture> _texture) :
	m_albedo(_texture)
	{ }
	
	virtual bool scatter(const Rayd& ray, const Hit_record& hit_record,
	vec3d& attenuation, Rayd& scattered) const override {
		scattered = Rayd(hit_record.point, vec3d::random_in_unit_sphere(), ray.time());
		
		attenuation = m_albedo->at(hit_record.u, hit_record.v, hit_record.point);
		
		return true;
	}
	
private:
	
	std::shared_ptr <tex::Texture> m_albedo;
	
};
