#include "Camera.h"

Camera::Camera(){
	this->cameraPosition = Vector3D();
	this->cameraDirection = Vector3D(0.0, 0.0, 1.0);
	this->cameraRight = Vector3D();
	this->cameraDown = Vector3D();
}

Camera::Camera(Vector3D pos, Vector3D dir, Vector3D right, Vector3D down){
	this->cameraPosition = pos;
	this->cameraDirection = dir;
	this->cameraRight = right;
	this->cameraDown = down;
}

Vector3D Camera::getCameraDirection() const {
	return this->cameraDirection;
}

Vector3D Camera::getCameraPosition() const {
	return this->cameraPosition;
}

Vector3D Camera::getCameraRight() const {
	return this->cameraRight;
}

Vector3D Camera::getCameraDown() const {
	return this->cameraDown;
}

void Camera::set(Vector3D pos, Vector3D dir, Vector3D right, Vector3D down){
	this->cameraPosition = pos;
	this->cameraDirection = dir;
	this->cameraRight = right;
	this->cameraDown = down;
}