// perlin noise utility

#pragma once

#include "util/util.h"

class Perlin {
	
public:
	
	Perlin();
	
	~Perlin();
	
	double noise(const vec3d& point) const;
	
	double turbulence(const vec3d& point, int depth = 7) const;
	
private:
	
	static constexpr const int m_point_count = 256;
	
	vec3d* m_random_vectors;
	
	int* m_px;
	int* m_py;
	int* m_pz;
	
private:
	
	static int* m_generate_permutation();
	
	static void m_permute(int n, int* p);
	
	static double m_trilinear_interpolate(double c[2][2][2], double u, double v, double w);
	
	static double m_perlin_interpolate(vec3d c[2][2][2], double u, double v, double w);
	
};
