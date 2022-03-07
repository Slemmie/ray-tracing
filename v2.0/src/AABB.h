// AABB collision logic

#pragma once

#include "util/util.h"
#include "ray.h"

class AABB {
	
public:
	
	constexpr AABB() { }
	
	constexpr AABB(const vec3d& _u, const vec3d& _v) :
	m_minv(_u),
	m_maxv(_v)
	{ }
	
	bool hit(const Rayd& ray, double t_min, double t_max) const;
	
	constexpr const vec3d& minv() const { return m_minv; }
	constexpr const vec3d& maxv() const { return m_maxv; }
	
private:
	
	vec3d m_minv;
	vec3d m_maxv;
	
};

AABB sorrounding_box(AABB box0, AABB box1);
