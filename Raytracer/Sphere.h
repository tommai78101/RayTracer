#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Vector3D.h"
#include "Color.h"
#include "Object.h"

#include <cmath>

class Sphere : public Object {
private:
	Vector3D center;
	double radius;
public:
	Sphere();
	Sphere(Vector3D center, double radius, Color color);

	Vector3D getCenter() const;
	double getRadius() const;

	double findIntersection(Ray ray);
	Color getColor() const;
	Vector3D getNormalAt(Vector3D point);
};

#endif