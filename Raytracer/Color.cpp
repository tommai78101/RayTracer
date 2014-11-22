#include "Color.h"

Color::Color(){
	this->red = 0.5;
	this->green = 0.5;
	this->blue = 0.5;
	this->spectral = 0.0;
}

Color::Color(double r, double g, double b, double s){
	this->red = r;
	this->green = g;
	this->blue = b;
	this->spectral = s;
}
	
double Color::getRed() const{
	return this->red;
}

double Color::getGreen() const{
	return this->green;
}

double Color::getBlue() const{
	return this->blue;
}

double Color::getSpectral() const{
	return this->spectral;
}

void Color::setRed(double r){
	this->red = r;
}

void Color::setGreen(double g){
	this->green = g;
}

void Color::setBlue(double b){
	this->blue = b;
}

void Color::setSpectral(double s){
	this->spectral = s;
}

double Color::getBrightness() const {
	return (this->red + this->green + this->blue)/3.0;
}

Color Color::makeColorScalar(double scalar) const {
	return Color(this->red * scalar, this->green * scalar, this->blue * scalar, this->spectral);
}

Color Color::addColor(Color color) const {
	return Color(this->red + color.getRed(), this->green + color.getGreen(), this->blue + color.getBlue(), this->spectral);
}

Color Color::multiplyColor(Color color) const {
	return Color(this->red * color.getRed(), this->green * color.getGreen(), this->blue * color.getBlue(), this->spectral);
}

Color Color::averageColor(Color color) const {
	return Color((this->red + color.getRed())/2, (this->green + color.getGreen())/2, (this->blue + color.getBlue())/2, this->spectral);
}

Color Color::clip(){
	double allColors = this->red + this->green + this->blue;
	double excessLight = allColors - 3;
	if (excessLight > 0.0){
		this->red += excessLight * (this->red / allColors);
		this->green += excessLight * (this->green / allColors);
		this->blue += excessLight * (this->blue / allColors);
	}
	if (this->red > 1.0) this->red = 1.0;
	if (this->red < 0.0) this->red = 0.0;
	if (this->green > 1.0) this->green = 1.0;
	if (this->green < 0.0) this->green = 0.0;
	if (this->blue > 1.0) this->blue = 1.0;
	if (this->blue < 0.0) this->blue = 0.0;

	return Color(this->red, this->green, this->blue, this->spectral);
}