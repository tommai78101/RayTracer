#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D(){
	this->x = this->y = this->z = 0.0;
}

Vector3D::Vector3D(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3D::getX() const {
	return this->x;
}

double Vector3D::getY() const {
	return this->y;
}

double Vector3D::getZ() const {
	return this->z;
}

Vector3D Vector3D::normalize(){
	double magnitude = this->magnitude();
	return Vector3D(this->x / magnitude, this->y / magnitude, this->z / magnitude);
}

double Vector3D::magnitude(){
	return std::sqrt((this->x*this->x) + (this->y*this->y) + (this->z*this->z));
}

Vector3D Vector3D::negate(){
	return Vector3D(-this->x, -this->y, -this->z);
}

double Vector3D::dotProduct(Vector3D vector){
	return this->x * vector.getX() + this->y * vector.getY() + this->z * vector.getZ();
}

Vector3D Vector3D::crossProduct(Vector3D vector){
	return Vector3D(
		this->y*vector.getZ() - this->z*vector.getY(),
		this->z*vector.getX() - this->x*vector.getZ(),
		this->x*vector.getY() - this->y*vector.getX());
}

Vector3D Vector3D::add(Vector3D vector){
	return Vector3D(this->x + vector.getX(), this->y + vector.getY(), this->z + vector.getZ());
}

Vector3D Vector3D::multiply(double scalar){
	return Vector3D(this->x * scalar, this->y * scalar, this->z * scalar);
}