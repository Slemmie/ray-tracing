// the ray class!

#pragma once

#include "util/vec3.h"

template <typename A> class Ray;

typedef Ray <long double> Rayld;
typedef Ray <double> Rayd;
typedef Ray <float> Rayf;

template <typename A = double> class Ray {
	
public:
	
	constexpr Ray() { }
	
	constexpr Ray(const vec3 <A>& _origin, const vec3 <A>& _direction) :
	m_origin(_origin),
	m_direction(_direction)
	{ }
	
	constexpr const vec3 <A> & origin() const { return m_origin; }
	inline vec3 <A> & origin() { return m_origin; }
	
	constexpr const vec3 <A> & direction() const { return m_direction; }
	inline vec3 <A> & direction() { return m_direction; }
	
	template <typename B> constexpr vec3 <A> at(const B& t) const { return origin() + t * direction(); }
	
private:
	
	vec3 <A> m_origin;
	
	vec3 <A> m_direction;
	
};

#include "ray.hpp"
