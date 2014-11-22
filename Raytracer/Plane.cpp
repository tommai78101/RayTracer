#include "Plane.h"

#include <iostream>

Plane::Plane(){
	this->normal = Vector3D(0.0, 1.0, 0.0);
	this->distanceFromOrigin = 0.0;
	this->setColor(Color(0.5, 0.5, 0.5, 0.0));
}

Plane::Plane(Vector3D normal, double distance, Color color){
	this->normal = normal;
	this->distanceFromOrigin = distance;
	this->setColor(color);
}

Vector3D Plane::getNormal() const {
	return this->normal;
}

double Plane::getDistanceFromOrigin() const {
	return this->distanceFromOrigin;
}

Vector3D Plane::getNormalAt(Vector3D position){
	return this->normal;
}

double Plane::findIntersection(Ray ray){
	Vector3D rayDirection = ray.getDirection();
	double a = rayDirection.dotProduct(this->normal);
	if (a == 0){
		//Ray is parallel to the plane.
		return -1.0;
	}
	else {
		double b = this->normal.dotProduct(ray.getOrigin().add(this->normal.multiply(this->distanceFromOrigin).negate()));
		return (-1*b)/a; //Distance from ray origin to point intersection.
	}
}

Color Plane::getColor() const {
	return this->color;
}