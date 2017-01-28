#ifndef INITIALIZATIONS_HPP
#define INITIALIZATIONS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Globals.hpp"
#include "objLoader.hpp"

struct TriangleVertex
{
	float x, y, z;
	float r, g, b;
};

HWND InitiateWindow(
	HINSTANCE hInstance,
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
);

void CreateObjectData(
	ID3D11Device* *Device,
	ID3D11Buffer* *VertexBuffer,
	ID3D11Buffer* *IndexBuffer,
	objData* *object,
	calcData *data
);

//void CreateTriangleData(
//	ID3D11Device* &Device,
//	ID3D11Buffer* &VertexBuffer,
//	TriangleVertex InputVertices[]
//);


#endif