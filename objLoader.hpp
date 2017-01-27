#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Globals.hpp"



// NECESSARY DATA STRUCTURES

struct objData
{
	DirectX::XMFLOAT3 vertex;
	DirectX::XMFLOAT3 normal;
};

struct calcData
{
	unsigned long *indices;
	int vertexCount;
	int indexCount;
};





class objLoader
{
private:
	objData* objData_teddy;
	calcData CalcRelatedData_teddy;

	int normal_count;
	int face_count;

	DirectX::XMFLOAT3 *faces;
	DirectX::XMVECTOR *vectorVertices;
	DirectX::XMVECTOR *vectorNormals;
	DirectX::XMVECTOR vector1, vector2, vector3;	// Vectors used for FACE calculations

	std::ifstream input_file;

public:
	objLoader();
	~objLoader();

	void importObjFile(std::string fileName);

	objData* *get_objData();
	calcData *get_calcData();
};

#endif