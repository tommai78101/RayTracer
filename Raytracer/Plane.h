#ifndef __PLANE_H__
#define __PLANE_H__

#include "Object.h"
#include "Color.h"
#include "Vector3D.h"
#include "Ray.h"

class Plane : public Object {
private:
	Vector3D normal;
	double distanceFromOrigin;
public:
	Plane();
	Plane(Vector3D normal, double distance, Color color);

	Vector3D getNormal() const;
	double getDistanceFromOrigin() const;

	Vector3D getNormalAt(Vector3D position);
	double findIntersection(Ray ray);
	Color getColor() const;
};

#endif