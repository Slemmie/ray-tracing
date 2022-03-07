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
	
	constexpr Ray(const vec3 <A>& _origin, const vec3 <A>& _direction, double _time = 0.0) :
	m_origin(_origin),
	m_direction(_direction),
	m_time(_time)
	{ }
	
	constexpr const vec3 <A>& origin() const { return m_origin; }
	inline vec3 <A>& origin() { return m_origin; }
	
	constexpr const vec3 <A>& direction() const { return m_direction; }
	inline vec3 <A>& direction() { return m_direction; }
	
	constexpr const double& time() const { return m_time; }
	inline double& time() { return m_time; }
	
	template <typename B> constexpr vec3 <A> at(const B& t) const { return origin() + t * direction(); }
	
private:
	
	vec3 <A> m_origin;
	
	vec3 <A> m_direction;
	
	double m_time;
	
};

#include "ray.hpp"
