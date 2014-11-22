#ifndef __RAY_H__
#define __RAY_H__

#include "Vector3D.h"

class Ray {
private:
	Vector3D origin, direction;
public:
	Ray();
	Ray(Vector3D o, Vector3D d);

	Vector3D getOrigin() const;
	Vector3D getDirection() const;
};

#endif