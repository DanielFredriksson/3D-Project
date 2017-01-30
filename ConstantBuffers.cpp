#include "ConstantBuffers.hpp"


void CreateSetConstantBuffers(
	MatrixBufferStored *ConstDataFloats,
	ID3D11Device* *Device,
	ID3D11DeviceContext* *DeviceContext,
	ID3D11Buffer* *ConstantBuffer
)
{
	// BUFFER DESCRIPTION: 'Settings'
	D3D11_BUFFER_DESC cbDesc;
	memset(&cbDesc, 0, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(MatrixBufferStored);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;						// Needs to be DYNAMIC so that we can Map/Unmap,
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			// via ~AlterConstantBuffers()
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// SUBRESOURCE DATA: Package the data
	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0, sizeof(InitData));
	InitData.pSysMem = ConstDataFloats;				// Meant to recieve data - not create.
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create buffer
	(*Device)->CreateBuffer(&cbDesc, &InitData, ConstantBuffer);

	// Set buffer
	(*DeviceContext)->GSSetConstantBuffers(0, 1, ConstantBuffer);

	// Flytta in float4x4 i SUBRESOURCE_DATA
	// SUBRESOURCE_DATA skickas in då vi skapar constant buffern.
	// Efter constant buffern har skapats så 'settas' den
}

void InitializeConstantMatrices(MatrixBufferLoaded *constData)	///Parameters are HARDCODED for the matrices except for VIEWMATRIX
{
	//WORLD MATRIX
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	//PROJECTION MATRIX
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(PI*0.45f,
		SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 60);

	//ROTATION MATRIX (270 degrees, X-Axis, Clockwise)
	DirectX::XMVECTOR X_Axis = { 1.0f, 0.0f, 0.0f };
	DirectX::XMMATRIX X_RotationMatrix = DirectX::XMMatrixRotationAxis(X_Axis, 270.0f);

	//ROTATION MATRIX (90 degrees, Y-Axis, Clockwise)
	DirectX::XMVECTOR Y_Axis = { 0.0f, 1.0f, 0.0f };
	DirectX::XMMATRIX Y_RotationMatrix = DirectX::XMMatrixRotationAxis(Y_Axis, -PI / 2);	// 100 cus haven't moved it.

																							//FLIP MATRIX
	DirectX::XMMATRIX Mirror_XZ, MoveUp, FlipY;
	Mirror_XZ = {
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
	MoveUp = {
		1.0f,	0.0f,	0.0f,	0.0f,
		1.0f,	2.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
	FlipY = DirectX::XMMatrixMultiply(Mirror_XZ, MoveUp);

	// HALF X-Y-Z
	DirectX::XMVECTOR tempHalf = {
		0.0f,
		0.0f,
		0.0f
	};

	///Here we supply the global constant buffer with data
	constData->world = worldMatrix;
	constData->projection = projectionMatrix;

	constData->RotationX270 = X_RotationMatrix;
	constData->RotationY90 = Y_RotationMatrix;

	constData->FlipMatrixY = FlipY;

	//constData->halfXYZ = tempHalf;
};

void MatrixToFloat4X4Reformat(
	MatrixBufferLoaded  *MatrixStruct,
	MatrixBufferStored	&FloatsStruct
)
{
	// Reformat to float4x4
	DirectX::XMStoreFloat4x4(&FloatsStruct.world, MatrixStruct->world);
	DirectX::XMStoreFloat4x4(&FloatsStruct.view, MatrixStruct->view);
	DirectX::XMStoreFloat4x4(&FloatsStruct.projection, MatrixStruct->projection);

	DirectX::XMStoreFloat4x4(&FloatsStruct.RotationX270, MatrixStruct->RotationX270);
	DirectX::XMStoreFloat4x4(&FloatsStruct.RotationY90, MatrixStruct->RotationY90);
	//DirectX::XMStoreFloat(&FloatsStruct.HalfXYZ, MatrixStruct->halfXYZ);
}



void AlterConstantBuffers(
	ID3D11Buffer* TargetBuffer,
	MatrixBufferStored TargetStruct,
	ID3D11DeviceContext* *DeviceContext
)
{
	D3D11_MAPPED_SUBRESOURCE MappedBuffer;
	(*DeviceContext)->Map(TargetBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedBuffer);
	memcpy(MappedBuffer.pData, &TargetStruct, sizeof(TargetStruct));
	(*DeviceContext)->Unmap(TargetBuffer, 0);
}