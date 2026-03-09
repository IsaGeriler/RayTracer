#include <iostream>

#include "colour.h"
#include "vec3.h"

int main(int argc, char** argv) {
	// Image Attributes
	unsigned int height = 256;
	unsigned int width = 256;

	// Render PPM
	std::cout << "P3\n" << height << ' ' << width << "\n255\n";

	for (unsigned int y = 0; y < height; y++) {
		std::clog << "\rScanlines remaining: " << (height - y) << ' ' << std::flush;
		for (unsigned int x = 0; x < width; x++) {
			auto pixelColour = colour(static_cast<float>(x) / (width - 1), static_cast<float>(y) / (width - 1), 0.f);
			writeColour(std::cout, pixelColour);
		}
	}
	std::clog << "\rDone.\n";
}