#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
	float min, max;

	interval() : min(+INFINITY), max(-INFINITY) {}
	interval(float _min, float _max) : min(_min), max(_max) {}

	float size() const { return max - min; }
	bool contains(float x) const { return min <= x && x <= max; }
	bool surrounds(float x) const { return min < x && x < max; }

	static const interval empty, universe;
};

const interval interval::empty = interval(+INFINITY, -INFINITY);
const interval interval::universe = interval(-INFINITY, +INFINITY);

#endif