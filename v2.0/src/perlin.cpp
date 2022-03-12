// implements perlin.h

#include "perlin.h"

#include <utility>
#include <cmath>

Perlin::Perlin() :
m_px(m_generate_permutation()),
m_py(m_generate_permutation()),
m_pz(m_generate_permutation())
{
	m_random_vectors = new vec3d[m_point_count];
	for (int i = 0; i < m_point_count; i++) {
		m_random_vectors[i] = vec3d::random(-1.0, 1.0).unit_vector();
	}
}

Perlin::~Perlin() {
	delete [] (m_random_vectors);
	delete [] (m_px);
	delete [] (m_py);
	delete [] (m_pz);
}

double Perlin::noise(const vec3d& point) const {
	double u = point.x() - floor(point.x());
	double v = point.y() - floor(point.y());
	double w = point.z() - floor(point.z());
	
	int i = static_cast <int> (floor(point.x()));
	int j = static_cast <int> (floor(point.y()));
	int k = static_cast <int> (floor(point.z()));
	
	vec3d c[2][2][2];
	
	for (int di = 0; di < 2; di++) {
		for (int dj = 0; dj < 2; dj++) {
			for (int dk = 0; dk < 2; dk++) {
				c[di][dj][dk] =
				m_random_vectors[m_px[(i + di) & 255] ^ m_py[(j + dj) & 255] ^ m_pz[(k + dk) & 255]];
			}
		}
	}
	
	return m_perlin_interpolate(c, u, v, w);
}

double Perlin::turbulence(const vec3d& point, int depth) const {
	double sum = 0.0;
	double wei = 1.0;
	vec3 tmp_p = point;
	
	for (int i = 0; i < depth; i++) {
		sum += wei * noise(tmp_p);
		tmp_p *= 2.0;
		wei *= 0.5;
	}
	
	return fabs(sum);
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

double Perlin::m_perlin_interpolate(vec3d c[2][2][2], double u, double v, double w) {
	double uu = u * u * (3.0 - 2.0 * u);
	double vv = v * v * (3.0 - 2.0 * v);
	double ww = w * w * (3.0 - 2.0 * w);
	
	double sum = 0.0;
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				vec3d wei(u - i, v - j, w - k);
				sum +=
				(uu * i + (1.0 - i) * (1.0 - uu)) *
				(vv * j + (1.0 - j) * (1.0 - vv)) *
				(ww * k + (1.0 - k) * (1.0 - ww)) *
				dot(c[i][j][k], wei);
			}
		}
	}
	
	return sum;
}
