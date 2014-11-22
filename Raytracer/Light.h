#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Color.h"
#include "Vector3D.h"
#include "Object.h"
#include "Source.h"

class Light : public Source {
private:
	Vector3D position;
	Color color;
public:
	Light();
	Light(Vector3D position, Color c);

	Vector3D getPosition() const;
	Color getColor() const;
};

#endif