#ifndef __COLOR_H__
#define __COLOR_H__

class Color {
private:
	double red, green, blue, spectral;
public:
	Color();
	Color(double r, double g, double b, double s);
	
	double getRed() const;
	double getGreen() const;
	double getBlue() const;
	double getSpectral() const;

	void setRed(double r);
	void setGreen(double g);
	void setBlue(double b);
	void setSpectral(double s);

	double getBrightness() const;
	Color makeColorScalar(double scalar) const;
	Color addColor(Color color) const;
	Color multiplyColor(Color color) const;
	Color averageColor(Color color) const;

	Color clip();

};

#endif