#pragma once

struct Vector
{
	float x;
	float y;
	float z;
};

struct VectorW
{
	Vector v;
	float w;
};

struct RotationMatrix
{
	VectorW x;
	VectorW y;
	VectorW z;
};

bool operator==(const Vector& a, const Vector& b);

bool operator==(const VectorW& a, const VectorW& b);

Vector operator+(const Vector& a, const Vector& b);

VectorW operator+(const VectorW& a, const VectorW& b);