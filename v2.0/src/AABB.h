// AABB collision logic

#pragma once

#include "util/util.h"

class AABB {
	
public:
	
	constexpr AABB() { }
	
	constexpr AABB(const vec3d& _u, const vec3d& _v) :
	m_minv(_u),
	m_maxv(_v)
	{ }
	
	bool hit(const Rayd& ray, double t_min, double t_max) const;
	
private:
	
	vec3d m_minv;
	vec3d m_maxv;
	
};
