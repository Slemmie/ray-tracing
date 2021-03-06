// simple vec3 class

#pragma once

#include "random.h"

#include <cassert>
#include <cmath>

template <typename A> class vec3;

typedef vec3 <long double> vec3ld;
typedef vec3 <double> vec3d;
typedef vec3 <float> vec3f;

template <typename A = double> class vec3 {
	
public:
	
	constexpr vec3() :
	m_data { (A) 0, (A) 0, (A) 0 }
	{ }
	
	constexpr vec3(A _v1, A _v2, A _v3) :
	m_data { _v1, _v2, _v3 }
	{ }
	
	constexpr const A& operator [] (int index) const {
		assert(index >= 0 && index < 3);
		return m_data[index];
	}
	
	constexpr A& operator [] (int index) {
		assert(index >= 0 && index < 3);
		return m_data[index];
	}
	
	inline const vec3 <A>& operator = (const vec3 <A>& other) {
		m_data[0] = other[0];
		m_data[1] = other[1];
		m_data[2] = other[2];
		return *this;
	}
	
	constexpr const A& x() const { return (*this)[0]; }
	constexpr          A& x()    { return (*this)[0]; }
	constexpr const A& r() const { return (*this)[0]; }
	constexpr          A& r()    { return (*this)[0]; }
	
	constexpr const A& y() const { return (*this)[1]; }
	constexpr          A& y()    { return (*this)[1]; }
	constexpr const A& g() const { return (*this)[1]; }
	constexpr          A& g()    { return (*this)[1]; }
	
	constexpr const A& z() const { return (*this)[2]; }
	constexpr          A& z()    { return (*this)[2]; }
	constexpr const A& b() const { return (*this)[2]; }
	constexpr          A& b()    { return (*this)[2]; }
	
	constexpr vec3 <A> operator - () const {
		return vec3 <A> (-m_data[0], -m_data[1], -m_data[2]);
	}
	
	template <typename B> inline vec3 <A>& operator += (const vec3 <B>& oth) {
		(*this)[0] += oth[0];
		(*this)[1] += oth[1];
		(*this)[2] += oth[2];
		return *this;
	}
	
	template <typename B> constexpr friend vec3 <A> operator + (vec3 <A> v1, const vec3 <B>& v2) {
		return v1 += v2;
	}
	
	template <typename B> inline vec3 <A>& operator -= (const vec3 <B> oth) {
		(*this)[0] -= oth[0];
		(*this)[1] -= oth[1];
		(*this)[2] -= oth[2];
		return *this;
	}
	
	template <typename B> constexpr friend vec3 <A> operator - (vec3 <A> v1, const vec3 <B>& v2) {
		return v1 -= v2;
	}
	
	template <typename B> inline vec3 <A>& operator *= (const B& c) {
		(*this)[0] *= c;
		(*this)[1] *= c;
		(*this)[2] *= c;
		return *this;
	}
	
	constexpr friend vec3 <A> operator * (vec3 <A> v, const A& c) {
		return v *= c;
	}
	
	constexpr friend vec3 <A> operator * (const A& c, vec3 <A> v) {
		return v *= c;
	}
	
	template <typename B> inline vec3 <A>& operator /= (const B& c) {
		(*this)[0] /= c;
		(*this)[1] /= c;
		(*this)[2] /= c;
		return *this;
	}
	
	constexpr friend vec3 <A> operator / (vec3 <A> v, const A& c) {
		return v /= c;
	}
	
	constexpr friend vec3 <A> operator / (const A& c, vec3 <A> v) {
		return v /= c;
	}
	
	template <typename B> inline vec3 <A>& operator *= (const vec3 <B>& v) {
		(*this)[0] *= v[0];
		(*this)[1] *= v[1];
		(*this)[2] *= v[2];
		return *this;
	}
	
	template <typename B> constexpr friend vec3 <A> operator * (vec3 <A> v1, const vec3 <B>& v2) {
		return v1 *= v2;
	}
	
	template <typename B> inline vec3 <A>& operator /= (const vec3 <B>& v) {
		(*this)[0] /= v[0];
		(*this)[1] /= v[1];
		(*this)[2] /= v[2];
		return *this;
	}
	
	template <typename B> constexpr friend vec3 <A> operator / (vec3 <A> v1, const vec3 <B>& v2) {
		return v1 /= v2;
	}
	
	template <typename B> constexpr friend A dot(const vec3 <A>& v1, const vec3 <B>& v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}
	
	template <typename B> constexpr friend vec3 <A> cross(const vec3 <A>& v1, const vec3 <B>& v2) {
		return vec3(
		v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]);
	}
	
	inline vec3 <A>& normalize() {
		(*this) /= length();
		return *this;
	}
	
	constexpr vec3 <A> unit_vector() const {
		return (*this) / length();
	}
	
	constexpr A length_squared() const {
		return (*this)[0] * (*this)[0] + (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2];
	}
	
	constexpr A length() const {
		return std::sqrt(length_squared());
	}
	
	static inline vec3 <A> random() {
		return vec3 <A>(Random::real(), Random::real(), Random::real());
	}
	
	static inline vec3 <A> random(A min, A max) {
		return vec3 <A>(Random::range(min, max), Random::range(min, max), Random::range(min, max));
	}
	
	static inline vec3 <A> random_in_unit_sphere() {
		while (true) {
			vec3 <A> point = vec3 <A>::random((A) -1, (A) 1);
			if (point.length_squared() < (A) 1) {
				return point;
			}
		}
	}
	
	static inline vec3 <A> random_unit_vec3() {
		return random_in_unit_sphere().normalize();
	}
	
	static inline vec3 <A> random_in_hemisphere(const vec3 <A>& normal) {
		vec3 <A> in_unit_sphere = random_in_unit_sphere();
		return dot(in_unit_sphere, normal) > 0.0 ? in_unit_sphere : -in_unit_sphere;
	}
	
	bool near_zero() const {
		constexpr double bound = 1e-8;
		return fabs((*this)[0] < bound) && fabs((*this)[1] < bound) && fabs((*this)[2] < bound);
	}
	
	static constexpr vec3 <A> reflect(const vec3 <A>& v, const vec3 <A>& n) {
		return v - (A) 2 * dot(v, n) * n;
	}
	
	static constexpr vec3 <A> refract(const vec3 <A>& uv, const vec3 <A>& n, double etai_over_etat) {
		auto cos_theta = fmin(dot(-uv, n), (A) 1);
		vec3 <A> r_out_perp = etai_over_etat * (uv + cos_theta * n);
		vec3 <A> r_out_parr = -sqrt(fabs((A) 1 - r_out_perp.length_squared())) * n;
		return r_out_perp + r_out_parr;
	}
	
	static constexpr vec3 random_in_unit_disk() {
		while (true) {
			auto point = vec3(Random::range(-1.0, 1.0), Random::range(-1.0, 1.0), 0);
			if (point.length_squared() < 1.0) {
				return point;
			}
		}
	}
	
private:
	
	A m_data[3];
	
};
