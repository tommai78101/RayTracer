#include "Ray.h"

Ray::Ray(){
	this->origin = Vector3D();
	this->direction = Vector3D(1.0, 0.0, 0.0);
}

Ray::Ray(Vector3D o, Vector3D d){
	this->origin = o;
	this->direction = d;
}

Vector3D Ray::getOrigin() const {
	return this->origin;
}

Vector3D Ray::getDirection() const {
	return this->direction;
}