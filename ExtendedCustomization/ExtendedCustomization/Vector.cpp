#include "Vector.h"

bool operator==(const Vector& a, const Vector& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator==(const VectorW& a, const VectorW& b)
{
	return a.v == b.v && a.w == b.w;
}

Vector operator+(const Vector& a, const Vector& b)
{
	Vector w;

	w.x = a.x + b.x;
	w.y = a.y + b.y;
	w.z = a.z + b.z;

	return w;
}

VectorW operator+(const VectorW& a, const VectorW& b)
{
	VectorW w;

	w.v = a.v + b.v;
	w.w = a.w;

	return w;
}