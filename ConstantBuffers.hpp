#ifndef CONSTANTBUFFERS_HPP
#define CONSTANTBUFFERS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Globals.hpp"
//#include "Camera.hpp"	// InitialiseConstantMatrices()

// GSConstantDataMatrices

struct MatrixBufferLoaded	///Defining WHAT it is that's being sent to the Geometry Shader
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	DirectX::XMMATRIX RotationX270;
	DirectX::XMMATRIX RotationY90;

	DirectX::XMMATRIX FlipMatrixY;
};

struct MatrixBufferStored
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	DirectX::XMFLOAT4X4 RotationX270;
	DirectX::XMFLOAT4X4 RotationY90;

	DirectX::XMFLOAT4X4 FlipMatrix;
};

void CreateSetConstantBuffers(
	MatrixBufferStored *ConstData,
	ID3D11Device* *Device,
	ID3D11DeviceContext* *DeviceContext,
	ID3D11Buffer* *ConstantBuffer
);

// Does not initialise the ViewMatrix, since Camera.UpdateCamera() does this.
//
void InitializeConstantMatrices(MatrixBufferLoaded *constData);	///Parameters are HARDCODED for the matrices except for VIEWMATRIX

void MatrixToFloat4X4Reformat(
	MatrixBufferLoaded  *MatrixStruct,
	MatrixBufferStored	&FloatsStruct
);


void AlterConstantBuffers(
	ID3D11Buffer* TargetBuffer,
	MatrixBufferStored TargetStruct,
	ID3D11DeviceContext* *DeviceContext
);



#endif