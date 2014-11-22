#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Ray.h"
#include "Color.h"

class Object {
protected:
	Color color;
public:
	Object();
	virtual Color getColor() const;
	void setColor(Color color);
	virtual double findIntersection(Ray ray);
	virtual Vector3D getNormalAt(Vector3D position);
};

#endif