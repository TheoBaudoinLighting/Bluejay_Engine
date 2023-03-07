// Author: Theo Baudoin
// Date: 2022-03-07
// Description: Ray class
#pragma once

#include "bje_vec3.h"

// Ray class
class ray
{
public:

	// Constructors
	ray()
	{
	}

	ray(const point3& origin, const vec3& direction, double time = 0.0, double radius = 0.0, double wavelength = 0.0)
		: orig(origin), dir(direction), tm(time), rad(radius), wavelen(wavelength)
	{
	}

	// Methods
	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }
	double radius() const { return rad; }
	double wavelength() const { return wavelen; }

	point3 at(double t) const
	{
		return orig + t * dir;
	}


	// Data
	point3 orig;
	vec3 dir;
	double tm;
	double rad;
	double wavelen;
};
