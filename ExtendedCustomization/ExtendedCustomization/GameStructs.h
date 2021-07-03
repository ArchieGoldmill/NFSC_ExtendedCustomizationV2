#pragma once
#include "Vector.h"

struct RideInfo
{
	int CarId;
	int data[285];
	int RenderUsage;
	int data1;
	float RideHeight;
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
	int* MeshPtr;
	void* CarShadowTexture;
};

struct Material
{
	int data[25];
	float DiffuseMinLevel;
	float DiffuseMinRed;
	float DiffuseMinGreen;
	float DiffuseMinBlue;
	float DiffuseMaxLevel;
	float DiffuseMaxRed;
	float DiffuseMaxGreen;
	float DiffuseMaxBlue;
	float DiffuseMinAlpha;
	float DiffuseMaxAlpha;
};

class FEObject;
struct FEMinList
{
	void* Vtable;
	int NumElements;
	FEObject* Head;
	FEObject* Tail;
};

struct FEPoint
{
	float H;
	float V;
};

struct FEObjectMouseState
{
	FEObject* Object;
	FEPoint Offset;
	int Flags;
};

struct FEPackage
{
	void* Vtable;
	FEObject* Next;
	FEObject* Prev;
	char* Name;
	int NameHash;
	int _data[12];
	FEMinList Objects;
	int _data1[22];
	FEObjectMouseState* MouseObjectStates;
	int NumMouseObjects;
};

class FEObject
{
public:
	void* Vtable;
	FEObject* Next;
	FEObject* Prev;
	int Guid;
	int NameHash;
	char* Name;
	int Type;
	int Flags;
	short RenderContext;
	short ResourceIndex;
	int Handle;
	int UserParam;
	int* pData;
	int DataSize;
	FEMinList Responses;
	FEMinList Scripts;
	int pCurrentScript;
	int Cached;
};

class FEGroup :public FEObject
{
public:
	FEMinList Children;
};