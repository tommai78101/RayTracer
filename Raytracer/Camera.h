#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3D.h"

class Camera {
private:
	Vector3D cameraPosition, cameraDirection, cameraRight, cameraDown;
public:
	Camera();
	Camera(Vector3D pos, Vector3D dir, Vector3D right, Vector3D down);

	Vector3D getCameraPosition() const;
	Vector3D getCameraDirection() const;
	Vector3D getCameraRight() const;
	Vector3D getCameraDown() const;

	void set(Vector3D pos, Vector3D dir, Vector3D right, Vector3D down);
};

#endif