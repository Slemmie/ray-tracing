// implements perlin.h

#include "perlin.h"

#include <utility>

Perlin::Perlin() :
m_px(m_generate_permutation()),
m_py(m_generate_permutation()),
m_pz(m_generate_permutation())
{
	m_random_doubles = new double[m_point_count];
	for (int i = 0; i < m_point_count; i++) {
		m_random_doubles[i] = Random::real();
	}
}

Perlin::~Perlin() {
	delete [] (m_random_doubles);
	delete [] (m_px);
	delete [] (m_py);
	delete [] (m_pz);
}

double Perlin::noise(const vec3d& point) const {
	int i = static_cast <int> (point.x() * 4.0) & 255;
	int j = static_cast <int> (point.y() * 4.0) & 255;
	int k = static_cast <int> (point.z() * 4.0) & 255;
	
	return m_random_doubles[m_px[i] ^ m_py[j] ^ m_pz[k]];
}

int* Perlin::m_generate_permutation() {
	int* arr = new int[m_point_count];
	
	for (int i = 0; i < m_point_count; i++) {
		arr[i] = i;
	}
	
	m_permute(m_point_count, arr);
	
	return arr;
}

void Perlin::m_permute(int n, int* p) {
	for (int i = n - 1; ~i; i--) {
		std::swap(p[Random::range_int(0, i)], p[i]);
	}
}
