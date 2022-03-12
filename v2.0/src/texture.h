// texture classes

#pragma once

#include "util/util.h"
#include "perlin.h"

#include <memory>

namespace tex {
	
	class Texture {
		
	public:
		
		virtual vec3d at(double u, double v, const vec3d& p) const = 0;
		
	};
	
	class Solid_color : public Texture {
		
	public:
		
		Solid_color() { }
		
		Solid_color(const vec3d& _color) :
		m_color(_color)
		{ }
		
		Solid_color(double _red, double _green, double _blue) :
		Solid_color(vec3d(_red, _green, _blue))
		{ }
		
		virtual vec3d at(double u, double v, const vec3d& p) const override {
			return m_color;
		}
		
	private:
		
		vec3d m_color;
		
	};
	
	class Checkers : public Texture {
		
	public:
		
		Checkers() { }
		
		Checkers(std::shared_ptr <Texture> _texture_even, std::shared_ptr <Texture> _texture_odd) :
		m_texture_even(_texture_even),
		m_texture_odd(_texture_odd)
		{ }
		
		Checkers(const vec3d& _color_even, const vec3d& _color_odd) :
		m_texture_even(std::make_shared <Solid_color> (_color_even)),
		m_texture_odd(std::make_shared <Solid_color> (_color_odd))
		{ }
		
		virtual vec3d at(double u, double v, const vec3d& p) const override {
			auto sines = sin(p.x() * 10.0) * sin(p.y() * 10.0) * sin(p.z() * 10.0);
			return sines < 0.0 ? m_texture_odd->at(u, v, p) : m_texture_even->at(u, v, p);
		}
		
	private:
		
		std::shared_ptr <Texture> m_texture_even;
		std::shared_ptr <Texture> m_texture_odd;
		
	};
	
	class Noise : public Texture {
		
	public:
		
		Noise() :
		m_scale(1.0)
		{ }
		
		Noise(double _scale) :
		m_scale(_scale)
		{ }
		
		virtual vec3d at(double u, double v, const vec3d& p) const override {
			return vec3(1.0, 1.0, 1.0) * 0.5 *
			(1.0 + sin(m_scale * p.z() + 10.0 * m_perlin.turbulence(p)));
		}
		
	private:
		
		Perlin m_perlin;
		
		double m_scale;
		
	};
	
} /// namespace tex
