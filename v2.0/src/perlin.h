// perlin noise utility

#pragma once

#include "util/util.h"

class Perlin {
	
public:
	
	Perlin();
	
	~Perlin();
	
	double noise(const vec3d& point) const;
	
private:
	
	static const int m_point_count = 256;
	
	double* m_random_doubles;
	
	int* m_px;
	int* m_py;
	int* m_pz;
	
private:
	
	static int* m_generate_permutation();
	
	static void permute(int n, int* p);
	
};
