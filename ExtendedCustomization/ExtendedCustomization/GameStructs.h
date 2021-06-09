#pragma once
#include "Vector.h"

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