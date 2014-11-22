#include "Object.h"

#include <iostream>

Object::Object(){
}

Color Object::getColor() const {
	return Color(0.0, 0.0, 0.0, 0.0);
}

double Object::findIntersection(Ray ray){
	return 0.0;
}

void Object::setColor(Color c){
	this->color = c;
}

Vector3D Object::getNormalAt(Vector3D position){
	return Vector3D(0.0, 0.0, 0.0);
}