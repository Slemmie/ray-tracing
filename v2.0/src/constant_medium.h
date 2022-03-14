// simple mediums like fog and clouds

#pragma once

#include "util/util.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class AABB;

class Constant_medium : public Hittable {
	
public:
	
	Constant_medium(std::shared_ptr <Hittable> _boundary,
	double _density, std::shared_ptr <tex::Texture> _texture) :
	m_boundary(_boundary),
	m_phase_function(std::make_shared <Isotropic> (_texture)),
	m_neg_inv_density(-1.0 / _density)
	{ }
	
	Constant_medium(std::shared_ptr <Hittable> _boundary, double _density, const vec3d& _color) :
	m_boundary(_boundary),
	m_phase_function(std::make_shared <Isotropic> (_color)),
	m_neg_inv_density(-1.0 / _density)
	{ }
	
	virtual bool hit(const Rayd& ray, double t_min, double t_max, Hit_record& hit_record) const override;
	
	virtual bool bounding_box(double time_begin, double time_end, AABB& output_box) const override {
		return m_boundary->bounding_box(time_begin, time_end, output_box);
	}
	
private:
	
	std::shared_ptr <Hittable> m_boundary;
	
	std::shared_ptr <Material> m_phase_function;
	
	double m_neg_inv_density;
	
};
