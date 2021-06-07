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

struct RideInfo
{
	int CarId;
};

struct CarRenderInfo
{
	char _data[0x370];
	Vector BoundingBoxMin;
	float NeonBrightness;
	Vector BoundingBoxMax;
	float NeonDirection;
	char _data1[0x60];
	RideInfo* RideInfo;
};