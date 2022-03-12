// texture classes

#pragma once

#include "util/util.h"

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
		
		Solid_color(double red, double green, double blue) :
		Solid_color(vec3d(red, green, blue))
		{ }
		
		virtual vec3d at(double u, double v, const vec3d& p) const override {
			return m_color;
		}
		
	private:
		
		vec3d m_color;
		
	};
	
} /// namespace tex
