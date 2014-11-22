#include "Light.h"

Light::Light(){
	this->position = Vector3D(0.0, 0.0, 0.0);
	this->color = Color(1.0, 1.0, 1.0, 0.0);
}

Light::Light(Vector3D position, Color c){
	this->position = position;
	this->color.setRed(c.getRed());
	this->color.setGreen(c.getGreen());
	this->color.setBlue(c.getBlue());
	this->color.setSpectral(c.getSpectral());
}

Vector3D Light::getPosition() const{
	return this->position;
}

Color Light::getColor() const{
	return this->color;
}