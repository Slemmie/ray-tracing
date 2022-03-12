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
	double u = point.x() - floor(point.x());
	double v = point.y() - floor(point.y());
	double w = point.z() - floor(point.z());
	
	u = u * u * (3.0 - 2.0 * u);
	v = v * v * (3.0 - 2.0 * v);
	w = w * w * (3.0 - 2.0 * w);
	
	int i = static_cast <int> (floor(point.x()));
	int j = static_cast <int> (floor(point.y()));
	int k = static_cast <int> (floor(point.z()));
	
	double c[2][2][2];
	
	for (int di = 0; di < 2; di++) {
		for (int dj = 0; dj < 2; dj++) {
			for (int dk = 0; dk < 2; dk++) {
				c[di][dj][dk] =
				m_random_doubles[m_px[(i + di) & 255] ^ m_py[(j + dj) & 255] ^ m_pz[(k + dk) & 255]];
			}
		}
	}
	
	return m_trilinear_interpolate(c, u, v, w);
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

double Perlin::m_trilinear_interpolate(double c[2][2][2], double u, double v, double w) {
	double sum = 0.0;
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				sum +=
				(i * u + (1 - i) * (1 - u)) *
				(j * v + (1 - j) * (1 - v)) *
				(k * w + (1 - k) * (1 - w)) *
				c[i][j][k];
			}
		}
	}
	
	return sum;
}
