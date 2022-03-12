// texture classes

#pragma once

#include "util/util.h"
#include "perlin.h"
#include "util/stb_image_impl.h"

#include <memory>
#include <iostream>
#include <algorithm>

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
	
	class Image : public Texture {
		
	public:
		
		Image() :
		m_data(nullptr),
		m_width(0),
		m_height(0),
		m_bytes_per_scanline(0)
		{ }
		
		Image(const char* filepath) {
			int comp_per_pixel = m_bytes_per_pixel;
			
			m_data = stbi_load(filepath, &m_width, &m_height, &comp_per_pixel, comp_per_pixel);
			
			if (!m_data) {
				std::cerr << "[error]: failed to load image '" << filepath << "'" << std::endl;
				m_width = m_height = 0;
				return;
			}
			
			m_bytes_per_scanline = m_bytes_per_pixel * m_width;
		}
		
		~Image() {
			if (m_data) {
				delete m_data;
			}
		}
		
		virtual vec3d at(double u, double v, const vec3d& p) const override {
			if (!m_data) {
				return vec3(1.0, 0.0, 1.0);
			}
			
			u = std::clamp(u, 0.0, 1.0);
			v = 1.0 - std::clamp(v, 0.0, 1.0);
			
			int i = static_cast <int> (u * m_width);
			int j = static_cast <int> (v * m_height);
			
			i = std::min(i, m_width - 1);
			j = std::min(j, m_height - 1);
			
			static constexpr const double color_scale = 1.0 / 255.0;
			auto pixel = m_data + j * m_bytes_per_scanline + i * m_bytes_per_pixel;
			
			return color_scale * vec3d(pixel[0], pixel[1], pixel[2]);
		}
		
	private:
		
		static constexpr const int m_bytes_per_pixel = 3;
		
		unsigned char* m_data;
		
		int m_width, m_height;
		
		int m_bytes_per_scanline;
		
	};
	
} /// namespace tex
