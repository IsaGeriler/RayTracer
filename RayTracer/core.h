#pragma once

#ifndef CORE_H
#define CORE_H

// Common Libraries
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Common Headers
#include "colour.h"
#include "interval.h"
#include "point3.h"
#include "ray.h"
#include "vec3.h"

// Constants
const float inf = std::numeric_limits<float>::infinity();
const float pi = 3.1415927f;

// Utility Functions
inline float to_radian(float degree) { return degree * pi / 180.f; }

#endif