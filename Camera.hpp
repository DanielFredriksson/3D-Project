#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <d3d11.h>
#include <DirectXMath.h>
#include "ConstantBuffers.hpp"	// Needed for GSConstantDataFloats & AlterConstantBuffers()


class Camera {
private:
	float CameraMovementSpeed = 1.0;
	float CameraRotationSpeed = 0.02;
	DirectX::XMVECTOR CameraStartPosition = { 0, 0, -20 };

public:
	DirectX::XMVECTOR CameraPosition;
	DirectX::XMVECTOR CameraDirection;
	DirectX::XMVECTOR CameraUpDirection;

	Camera();		// maybe initialise GSConstantBUffer with original view here.
	~Camera();

	// Current controls:
	// 'w','a','s','d'	controls camera movement.
	// mouse cursor		controls camera direction.
	// 'r'				resets the camera.
	void UpdateCamera(
		TCHAR					CharacterMessage,
		POINT					MouseCoordinates,
		MatrixBufferStored	*FormattedStructData,
		ID3D11Buffer*			*GSConstantBuffer,
		ID3D11DeviceContext*	*DeviceContext
	);

	void ResetCamera();

	void MoveCameraLeft();
	void MoveCameraRight();
	void MoveCameraForward();
	void MoveCameraBackward();

	void MoveCameraUp();

	void RotateCameraVertically(POINT MouseMovement);
	void RotateCameraHorizontally(POINT mouseMovement);
};




#endif