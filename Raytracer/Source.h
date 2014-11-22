#ifndef __SOURCE_H__
#define __SOURCE_H__

#include "Vector3D.h"
#include "Color.h"

class Source {
public:
	Source();
	virtual Vector3D getPosition() const;
	virtual Color getColor() const;
};

#endif