#pragma once

#ifndef COLOUR_H
#define COLOUR_H

#include <iostream>

#include "vec3.h"

using colour = vec3;

void write_colour(std::ostream& out, const colour& pixelColour) {
	float r = pixelColour.x;
	float g = pixelColour.y;
	float b = pixelColour.z;

	int rbyte = int(255.f * r);
	int gbyte = int(255.f * g);
	int bbyte = int(255.f * b);

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif