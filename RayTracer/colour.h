#pragma once

#ifndef COLOUR_H
#define COLOUR_H

#include "interval.h"
#include "vec3.h"

using colour = vec3;

inline float to_gamma(float linear_comp) {
	return (linear_comp > 0) ? std::sqrtf(linear_comp) : 0.f;
}

void write_colour(std::ostream& out, const colour& pixelColour) {
	float r = pixelColour.x;
	float g = pixelColour.y;
	float b = pixelColour.z;

	// Replace NaN components with zero
	if (r != r) r = 0.f;
	if (g != g) g = 0.f;
	if (b != b) b = 0.f;

	// Apply gamma transform for gamma 2
	r = to_gamma(r);
	g = to_gamma(g);
	b = to_gamma(b);

	// Convers [0, 1] component values to the byte range [0, 255]
	static const interval intensity(0.f, 0.999f);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif