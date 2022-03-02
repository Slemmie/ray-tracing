// basic utility

#pragma once

#include <cmath>
#include <limits>

constexpr double INF = std::numeric_limits <double>::infinity();
constexpr double PI = 3.1415926535897932385;

constexpr double deg_to_rad(double degrees) {
	return degrees * PI / 180.0;
}

// as well as the rest of util/...

#include "vec3.h"
