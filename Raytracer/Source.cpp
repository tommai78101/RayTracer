#include "Source.h"

Source::Source(){
}

Vector3D Source::getPosition() const {
	return Vector3D(0.0, 0.0, 0.0);
}

Color Source::getColor() const {
	return Color(1.0, 1.0, 1.0, 0);
}