#include "Sphere.h"

#include <iostream>

Sphere::Sphere(){
	this->center = Vector3D(0.0, 0.0, 0.0);
	this->radius = 1.0;
	this->setColor(Color(0.5, 0.5, 0.5, 0));
}

Sphere::Sphere(Vector3D center, double radius, Color color){
	this->center = center;
	this->radius = radius;
	this->setColor(color);
}

Vector3D Sphere::getCenter() const {
	return this->center;
}

double Sphere::getRadius() const {
	return this->radius;
}

double Sphere::findIntersection(Ray ray){
	Vector3D rayOrigin = ray.getOrigin();
	double rayOriginX = rayOrigin.getX();
	double rayOriginY = rayOrigin.getY();
	double rayOriginZ = rayOrigin.getZ();

	Vector3D rayDirection = ray.getDirection();
	double rayDirectionX = rayDirection.getX();
	double rayDirectionY = rayDirection.getY();
	double rayDirectionZ = rayDirection.getZ();

	double centerX = this->center.getX();
	double centerY = this->center.getY();
	double centerZ = this->center.getZ();

	double a = 1.0; //Normalized.
	double b = (2*(rayOriginX - centerX)*rayDirectionX) + (2*(rayOriginY - centerY)*rayDirectionY) + (2*(rayOriginZ - centerZ)*rayDirectionZ);
	double c = std::pow(rayOriginX - centerX, 2) + std::pow(rayOriginY - centerY, 2) + std::pow(rayOriginZ - centerZ, 2) - (this->radius*this->radius);

	double discriminant = b*b - 4 *c; //b^2 -4ac

	if (discriminant > 0){
		//The ray intersects the sphere, intersecting two sides.

		//First root
		double root1 = ((-1*b - std::sqrt(discriminant))/2) - 0.00001;
		if (root1 > 0){
			//The first root is the smallest positive root.
			return root1;
		}
		else {
			//The second root is the smallest positive root.
			double root2 = ((std::sqrt(discriminant) - b)/2) - 0.00001;
			return root2;
		}
	}
	else {
		//The ray missed the sphere.
		return -1;
	}
}

Vector3D Sphere::getNormalAt(Vector3D point) {
	//Normal always points away from center of sphere.
	Vector3D normal = point.add(this->center.negate()).normalize();
	return normal;
}

Color Sphere::getColor() const{
	return this->color;
}