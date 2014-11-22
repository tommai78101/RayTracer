#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

class Vector3D {
private:
	double x, y, z;
public:
	Vector3D();
	Vector3D(double x, double y, double z);

	double getX() const;
	double getY() const;
	double getZ() const;

	//Linear algebra operations
	Vector3D normalize();
	double magnitude();
	Vector3D negate();
	double dotProduct(Vector3D vector);
	Vector3D crossProduct(Vector3D vector);
	Vector3D add(Vector3D vector);
	Vector3D multiply(double scalar);
};

#endif