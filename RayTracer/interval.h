#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
	float min, max;

	interval() : min(+INFINITY), max(-INFINITY) {}
	interval(float _min, float _max) : min(_min), max(_max) {}

	interval(const interval& a, const interval& b) {
		min = (a.min <= b.min) ? a.min : b.min;
		max = (a.max <= b.max) ? a.max : b.max;
	}

	float size() const { return max - min; }
	float clamp(float x) const { return std::fmin(max, std::fmax(x, min)); }
	bool contains(float x) const { return min <= x && x <= max; }
	bool surrounds(float x) const { return min < x && x < max; }

	interval expand(float delta) const {
		float padding = delta / 2.f;
		return interval(min - padding, max + padding);
	}

	static const interval empty, universe;
};

const interval interval::empty = interval(+INFINITY, -INFINITY);
const interval interval::universe = interval(-INFINITY, +INFINITY);

#endif