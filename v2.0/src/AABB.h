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
	
	constexpr const vec3d& minv() const { return m_minv; }
	constexpr const vec3d& maxv() const { return m_maxv; }
	
private:
	
	vec3d m_minv;
	vec3d m_maxv;
	
};

AABB sorrounding_box(AABB box0, AABB box1) {
	vec3d smaller(
	fmin(box0.minv().x(), box1.minv().x()),
	fmin(box0.minv().y(), box1.minv().y()),
	fmin(box0.minv().z(), box1.minv().z())
	);
	
	vec3d bigger(
	fmax(box0.maxv().x(), box1.maxv().x()),
	fmax(box0.maxv().y(), box1.maxv().y()),
	fmax(box0.maxv().z(), box1.maxv().z())
	);
	
	return AABB(smaller, bigger);
}
