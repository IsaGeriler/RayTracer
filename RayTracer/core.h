#pragma once

#ifndef CORE_H
#define CORE_H

// Common Libraries
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// Constants
const float inf = std::numeric_limits<float>::infinity();
const float pi = 3.1415927f;

// Utility Functions
inline float random_float() {
	static std::uniform_real_distribution<float> dist(0.f, 1.f);
	static std::mt19937 gen;
	return dist(gen);
}

inline float random_float(float min, float max) { return min + (max - min) * random_float(); }
inline float to_radian(float degree) { return degree * pi / 180.f; }

// Common Headers
#include "colour.h"
#include "interval.h"
#include "point3.h"
#include "ray.h"
#include "vec3.h"

#endif