#ifdef _WIN32
#  define _CRT_SECURE_NO_DEPRECATE
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "Vector3D.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"

const double PI = 3.14159265358979323846;

struct RGBType {
	//No alpha.
	double r;
	double g;
	double b;
};

void saveBMP(const std::string filename, int w, int h, int dpi, RGBType* data){
	FILE* file;
	int totalSize = w * h;
	int stride = 4 * totalSize;
	int fileSize = 54 + stride;

	double factor = 39.375;
	int meter = static_cast<int>(factor);
	int pixelsPerMeter = dpi * meter;

	unsigned char bmpFileHeader[14] = {'B', 'M', 0,0,0,0, 0,0,0,0, static_cast<char>(54),0,0,0 };
	unsigned char bmpInfoHeader[40];

	std::memset(bmpInfoHeader, 0, sizeof(bmpInfoHeader));

	bmpInfoHeader[0] = static_cast<char>(40);
	bmpInfoHeader[12] = 1;
	bmpInfoHeader[14] = 24;

	//Header: Signature, Filesize, Reserved1, Reserved2, File Offset to Pixel Array
	bmpFileHeader[2] = (unsigned char)(fileSize);
	bmpFileHeader[3] = (unsigned char)(fileSize >> 8);
	bmpFileHeader[4] = (unsigned char)(fileSize >> 16);
	bmpFileHeader[5] = (unsigned char)(fileSize >> 24);

	//Width of the bitmap
	bmpInfoHeader[4] = (unsigned char)(w);
	bmpInfoHeader[5] = (unsigned char)(w>>8);
	bmpInfoHeader[6] = (unsigned char)(w>>16);
	bmpInfoHeader[7] = (unsigned char)(w>>24);

	//Height of the bitmap
	bmpInfoHeader[8] = (unsigned char)(h);
	bmpInfoHeader[9] = (unsigned char)(h>>8);
	bmpInfoHeader[10] = (unsigned char)(h>>16);
	bmpInfoHeader[11] = (unsigned char)(h>>24);

	//Pixels per meter Density
	bmpInfoHeader[21] = (unsigned char)(pixelsPerMeter);
	bmpInfoHeader[22] = (unsigned char)(pixelsPerMeter>>8);
	bmpInfoHeader[23] = (unsigned char)(pixelsPerMeter>>16);
	bmpInfoHeader[24] = (unsigned char)(pixelsPerMeter>>24);

	bmpInfoHeader[25] = (unsigned char)(pixelsPerMeter);
	bmpInfoHeader[26] = (unsigned char)(pixelsPerMeter>>8);
	bmpInfoHeader[27] = (unsigned char)(pixelsPerMeter>>16);
	bmpInfoHeader[28] = (unsigned char)(pixelsPerMeter>>24);

	bmpInfoHeader[29] = (unsigned char)(pixelsPerMeter);
	bmpInfoHeader[30] = (unsigned char)(pixelsPerMeter>>8);
	bmpInfoHeader[31] = (unsigned char)(pixelsPerMeter>>16);
	bmpInfoHeader[32] = (unsigned char)(pixelsPerMeter>>24);

	file = std::fopen(filename.c_str(), "wb"); //wb := 'W'rite 'B'inary

	std::fwrite(bmpFileHeader, 1, 14, file);
	std::fwrite(bmpInfoHeader, 1, 40, file);

	for (int i =0; i<totalSize; i++){
		RGBType rgb = data[i];

		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;

		unsigned char color[3] = {
			static_cast<unsigned char>((int) std::floor(blue)), 
			static_cast<unsigned char>((int) std::floor(green)),
			static_cast<unsigned char>((int) std::floor(red))
		};

		std::fwrite(color, 1, 3, file);
	}

	std::fclose(file);
}

Vector3D reflect(Vector3D rayDirection, Vector3D normal){
	double dotProduct = normal.dotProduct(rayDirection.negate());
	Vector3D result = normal.multiply(dotProduct);
	result = result.add(rayDirection);
	result = result.multiply(2.0);
	result = rayDirection.negate().add(result);
	result = result.normalize();
	return result;
}

int getWinningSceneObject(std::vector<double> intersections){
	//Returns the index of the winning intersection.
	int indexOfMinimumValue = -1;

	//Prevent unnecessary calculations.
	if (intersections.size() == 0){
		//If empty...
		return -1;
	}
	else if (intersections.size() == 1) {
		if (intersections.at(0) > 0){
			//If intersection is greater than zero, then return index.
			return 0; //Index, not intersection.
		}
		else {
			//Otherwise, the only intersection value is negative, which means the rays all missed target.
			return -1;
		}
	}
	else {
		//There are many intersections. First, find the max.
		double max = 0;
		for (std::vector<double, std::allocator<double>>::size_type i= 0; i<intersections.size(); i++){
			if (max < intersections.at(i)){
				max = intersections.at(i);
			}
		}

		//Then starting from max, find the min positive value.
		if (max > 0){
			//We only want positive intersections (Rays that have hit objects)
			for (std::vector<double, std::allocator<double>>::size_type i=0; i < intersections.size(); i++){
				if (intersections.at(i) > 0 && intersections.at(i) <= max){
					max = intersections.at(i);
					indexOfMinimumValue = i;
				}
			}
			return indexOfMinimumValue;
		}
		else {
			//All intersections are negative.
			return -1;
		}
	}
}

Color getColorAt(Vector3D position, Vector3D direction, std::vector<Object*> sceneObjects, int indexWinningObject, std::vector<Source*> lightSources, double ambient, double accuracy){
	Color winningObjectColor = sceneObjects.at(indexWinningObject)->getColor();
	Vector3D winningObjectNormal = sceneObjects.at(indexWinningObject)->getNormalAt(position);

	//Adding checkerboard to the plane.
	if ((int)(winningObjectColor.getSpectral()) == 2){
		int square = (int) std::floor(position.getX()) + (int) std::floor(position.getZ());
		if ((square % 2) == 0){
			//Black tile.
			winningObjectColor.setRed(0.0); 
			winningObjectColor.setGreen(0.0);
			winningObjectColor.setBlue(0.0);
		}
		else {
			//White tile.
			winningObjectColor.setRed(1.0); 
			winningObjectColor.setGreen(1.0);
			winningObjectColor.setBlue(1.0);
		}
	}


	Color finalColor = winningObjectColor.makeColorScalar(ambient);

	//Adding spectral reflection, or reflecting light more than 1 time.
	if (winningObjectColor.getSpectral() > 0 && winningObjectColor.getSpectral() <= 1){
		//Handle reflection from objects with specular intensity.
		/*double dotProductA = winningObjectNormal.dotProduct(direction.negate());
		Vector3D scalarA = winningObjectNormal.multiply(dotProductA);
		Vector3D addA = scalarA.add(direction);
		Vector3D scalarB = addA.multiply(2.0);
		Vector3D addB = direction.negate().add(scalarB);
		Vector3D reflectionDirection = addB.normalize();*/

		Vector3D reflectionDirection = reflect(direction, winningObjectNormal);
		Ray reflectionRay (position, reflectionDirection);

		//Determine what the ray intersects with first.
		std::vector<double> reflectionIntersections;

		for (std::vector<Object*, std::allocator<Object*>>::size_type ri = 0; ri < sceneObjects.size(); ri++){
			reflectionIntersections.push_back(sceneObjects.at(ri)->findIntersection(reflectionRay));
		}

		int indexReflectionWinningObject = getWinningSceneObject(reflectionIntersections);
		if (indexReflectionWinningObject != -1){
			//Reflection ray missed.
			if (reflectionIntersections.at(indexReflectionWinningObject) > accuracy){
				//Determine the position and direction at the point of intersection with the reflection ray.
				//The ray only affects the color if it reflects off something.
				Vector3D reflectionIntersectionPosition = position.add(reflectionDirection.multiply(reflectionIntersections.at(indexReflectionWinningObject)));
				Vector3D reflectionIntersectionDirection = reflectionDirection;

				//Reflections are recursive.
				Color reflectionIntersectionColor = getColorAt(reflectionIntersectionPosition, reflectionIntersectionDirection, sceneObjects, indexReflectionWinningObject, lightSources, ambient, accuracy);

				finalColor = finalColor.addColor(reflectionIntersectionColor.makeColorScalar(winningObjectColor.getSpectral()));
			}
		}
	}



	//Light reflection.
	for (std::vector<Source*, std::allocator<Source*>>::size_type i = 0; i < lightSources.size(); i++){
		//We get the normalized light vector.
		Vector3D lightDirection = lightSources.at(i)->getPosition().add(position.negate()).normalize();
		//We then calculate to see if the surface is facing towards the light source. If surface is perpendicular to the light vector, it will
		//not reflect the light vector back. If the surface is facing away from the light (angle of light vector and surface is larger than 90 degrees),
		//there will be no light reflections. Therefore, we only consider light reflections if the surface is looking at the light source (angle is less
		//than absolute 90 degrees, regardless if clockwise or counterclockwise).
		double cosAngle = winningObjectNormal.dotProduct(lightDirection);
		if (cosAngle > 0.0){
			//Test for shadows.
			Vector3D distanceFromLight = lightSources.at(i)->getPosition().add(position.negate()).normalize();
			double distanceFromLightMagnitude = distanceFromLight.magnitude();
			Ray shadowRay(position, distanceFromLight);
			std::vector<double> secondaryIntersections;
			bool hasShadows = false;
			//First, we need to find all intersections the shadow ray has detected for each scene objects.
			for (std::vector<Object*, std::allocator<Object*>>::size_type j = 0; j<sceneObjects.size() && !hasShadows; j++){
				secondaryIntersections.push_back(sceneObjects.at(j)->findIntersection(shadowRay));
			}

			for (std::vector<double, std::allocator<double>>::size_type j = 0; j < secondaryIntersections.size(); j++){
				if (secondaryIntersections.at(j) > accuracy){
					if (secondaryIntersections.at(j) <= distanceFromLightMagnitude){
						hasShadows = true;
					}
					break;
				}
			}
			if (!hasShadows){
				if (winningObjectColor.getSpectral() > 0.0 && winningObjectColor.getSpectral() <= 1.0){
					//Determines the tint of the light's reflection from the light source by blending (multiplying) the light source color and the
					//surface color, and then using the (dot product of the light vector and the normal vector of the surface) as a way to measure
					//the reflection intensity. This intensity determines how intense the tint of the blended color is.

					finalColor = finalColor.addColor(winningObjectColor.multiplyColor(lightSources.at(i)->getColor()).makeColorScalar(cosAngle));

					//specular := Shininess of the object. Also the specular intensity is determined by ((the dot product of the vector of light 
					//from source and the reflection vector of that same light when bounced off of a surface) to the power of some arbitrary value)
					//multiplied by (the spectral value of the surface that bounced off the light's reflection or that intersects the light 
					//direction vector).

					//double dotProductNormalDirection = winningObjectNormal.dotProduct(direction.negate());
					//Projection of direction (ray) onto the object's normal. End result is a vector.
					/*Vector3D projectionOnNormal = winningObjectNormal.multiply(dotProductNormalDirection); 
					Vector3D sideOppositeOfAngleBetweenNormalDirection = projectionOnNormal.add(direction); 
					Vector3D reflectedDistanceBetweenDirectionSourceAndDestination = sideOppositeOfAngleBetweenNormalDirection.multiply(2.0);
					Vector3D reflectedVector = direction.negate().add(reflectedDistanceBetweenDirectionSourceAndDestination);
					Vector3D reflectionDirection = reflectedVector.normalize();*/

					Vector3D reflectionDirection = reflect(direction, winningObjectNormal);
					Vector3D lightDirection = lightSources.at(i)->getPosition().add(position.negate()).normalize();
					double specular = reflectionDirection.dotProduct(lightDirection);
					if (specular > 0.0){
						specular = std::pow(specular, 10.0);
						finalColor = finalColor.addColor(lightSources.at(i)->getColor().makeColorScalar(specular * winningObjectColor.getSpectral()));
					}
				}
				else {
					finalColor = finalColor.addColor(winningObjectColor.multiplyColor(lightSources.at(i)->getColor()).makeColorScalar(cosAngle));
				}
			}
		}
	}
	return finalColor.clip();
}



void fillBackgroundColor(RGBType* pixels, int w, int h, int r, int g, int b, Camera cam, Vector3D camDirection, Vector3D camRight, Vector3D camDown, std::vector<Object*> sceneObjects, std::vector<Source*> lightSources, double ambientLight, double accuracy){
	float aspectRatio = (float) w / (float) h;
	double xAmount, yAmount;

	//Anti-aliasing depth.
	const int antiAliasingDepth = 6;
	double antiAliasingThreshold = 0.1;
	int aaIndex = 0;
	for (int y = 0; y<h; y++){
		for (int x =0; x < w; x++){
			int pixelsPointer = x + y * w;

			//Anti-aliasing.
			//Start with blank pixels.
			double tempRed[antiAliasingDepth*antiAliasingDepth];
			double tempGreen[antiAliasingDepth*antiAliasingDepth];
			double tempBlue[antiAliasingDepth*antiAliasingDepth];

			for (int aax = 0; aax < antiAliasingDepth; aax++){
				for (int aay = 0; aay < antiAliasingDepth; aay++) {

					aaIndex = aay*antiAliasingDepth + aax;
					//Create ray from camera to pixel.
					if (antiAliasingDepth <= 1){
						//No Anti-aliasing.
						if (w > h){
							//Image is wider.
							xAmount = ((x+0.5) / w) * aspectRatio - (((w - h)/(double) h)/2);
							yAmount = ((h - y) + 0.5)/h;
						}
						else if (h > w){
							//Image is taller.
							xAmount = (x+0.5) / w;
							yAmount = (((h - y) + 0.5)/h/aspectRatio) - (((h - w)/(double) w)/2);
						}
						else {
							//Image is square.
							xAmount = (x+0.5)/w;
							yAmount = ((h -y) + 0.5)/h;
						}
					}
					else {
						//Anti-aliasing.
						if (w > h){
							//Image is wider.
							xAmount = ((x+((double)aax/(double)antiAliasingDepth-1)) / w) * aspectRatio - (((w - h)/(double) h)/2);
							yAmount = ((h - y) + 0.5)/h;
						}
						else if (h > w){
							//Image is taller.
							xAmount = (x+((double)aax/(double)antiAliasingDepth-1)) / w;
							yAmount = (((h - y) + 0.5)/h/aspectRatio) - (((h - w)/(double) w)/2);
						}
						else {
							//Image is square.
							xAmount = (x+((double)aax/(double)antiAliasingDepth-1))/w;
							yAmount = ((h -y) + ((double)aax/(double)antiAliasingDepth-1))/h;
						}
					}

					Vector3D cameraRayOrigin = cam.getCameraPosition();
					Vector3D cameraRayDirection = camDirection.add(camRight.multiply(xAmount - 0.5).add(camDown.multiply(yAmount - 0.5))).normalize();

					Ray cameraRay(cameraRayOrigin, cameraRayDirection);

					std::vector<double> intersections;

					for (std::vector<double, std::allocator<double>>::size_type i= 0; i<sceneObjects.size(); i++){
						intersections.push_back(sceneObjects.at(i)->findIntersection(cameraRay));
					}

					//TODO: What does this actually do?
					int indexWinningObject = getWinningSceneObject(intersections);
					if (indexWinningObject == -1){
						tempRed[aaIndex]   = 0.0;
						tempGreen[aaIndex] = 0.0;
						tempBlue[aaIndex]  = 0.0;
					}
					else {
						//Index corresponds to an object in the scene
						if (intersections.at(indexWinningObject) > accuracy){
							//Determine the position and direction vectors at the point of intersection.
							Vector3D intersectionPosition = cameraRayOrigin.add(cameraRayDirection.multiply(intersections.at(indexWinningObject)));
							Vector3D intersectionDirection = cameraRayDirection;

							Color intersectionColor = getColorAt(intersectionPosition, intersectionDirection, sceneObjects, indexWinningObject, lightSources, ambientLight, accuracy);
							//Color of the object: Color winningObjectColor = sceneObjects.at(indexWinningObject)->getColor();
							tempRed[aaIndex] = intersectionColor.getRed();
							tempGreen[aaIndex] = intersectionColor.getGreen();
							tempBlue[aaIndex] = intersectionColor.getBlue();
						}
					}
				}
			}
			//average the pixel color
			double totalRed = 0.0;
			double totalGreen = 0.0;
			double totalBlue = 0.0;

			for (int i = 0; i < antiAliasingDepth*antiAliasingDepth; i++){
				totalRed += tempRed[i];
				totalGreen += tempGreen[i];
				totalBlue += tempBlue[i];
			}

			double averageRed = totalRed/(antiAliasingDepth*antiAliasingDepth);
			double averageGreen = totalGreen/(antiAliasingDepth*antiAliasingDepth);
			double averageBlue = totalBlue/(antiAliasingDepth*antiAliasingDepth);

			pixels[pixelsPointer].r = averageRed;
			pixels[pixelsPointer].g = averageGreen;
			pixels[pixelsPointer].b = averageBlue;
		}
	}
}


int main(int argCount, char* argValues[]){
	std::cout << "Rendering..." << std::endl;

	//Rendering time.
	std::clock_t lastTime, now;
	lastTime = std::clock();


	int dpi = 72;
	//Higher values for higher resolutions.
	int width = 320;
	int height = 240;
	int total = width * height;
	RGBType* pixels = new RGBType[total];

	Vector3D origin(0.0, 0.0, 0.0);
	Vector3D X_axis(1.0, 0.0, 0.0);
	Vector3D Y_axis(0.0, 1.0, 0.0);
	Vector3D Z_axis(0.0, 0.0, 1.0);

	Vector3D lookAt(0.0, 0.0, 0.0);
	Vector3D cameraPosition(3.0, 1.5, -4.0);
	Vector3D differenceInBetween(
		cameraPosition.getX() - lookAt.getX(),
		cameraPosition.getY() - lookAt.getY(), 
		cameraPosition.getZ() - lookAt.getZ());
	Vector3D cameraDirection = differenceInBetween.negate().normalize();
	Vector3D cameraRight = Y_axis.crossProduct(cameraDirection).normalize();
	Vector3D cameraDown = cameraRight.crossProduct(cameraDirection);

	Camera sceneCamera(cameraPosition, cameraDirection, cameraRight, cameraDown);

	Color whiteLight (1.0, 1.0, 1.0, 0.0);
	Color prettyGreen (0.5, 1.0, 0.5, 0.3);
	Color checkerboard(0.5, 0.25, 0.25, 2.0);
	Color maroon(0.5, 0.25, 0.25, 0.6);
	Color metal(0.7, 0.7, 0.85, 0.9);
	Color gray(0.5, 0.5, 0.5, 0.0);
	Color black(0.0, 0.0, 0.0, 0.0);

	Vector3D lightPosition(-7.0,10,-10.0);
	Light sceneLight (lightPosition, whiteLight);
	std::vector<Source*> lightSources;
	lightSources.push_back(dynamic_cast<Source*>(&sceneLight));

	double ambientLight = 0.2;
	double accuracy = 0.00001;

	//Scene objects.
	Sphere sceneSphere(origin, 1, prettyGreen);
	Sphere sceneSphere2(Vector3D(-3.0, 0.0, 0.0), 1, metal);
	Sphere sceneSphere3(Vector3D(1.5, 0.0, 2.0), 1, maroon);
	Plane scenePlane(Y_axis, -1.0, checkerboard);

	std::vector<Object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere2));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere3));
	sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane));

	fillBackgroundColor(pixels, width, height, 100, 149, 237, sceneCamera, cameraDirection, cameraRight, cameraDown, sceneObjects, lightSources, ambientLight, accuracy);

	saveBMP("test.bmp", width, height, dpi, pixels);

	delete pixels;

	now = std::clock();
	float dt = (((float) now - (float) lastTime) / 1000.0f);
	std::cout << "Total time: " << dt << " seconds." << std::endl; 
	std::system("pause");
	return 0;
}