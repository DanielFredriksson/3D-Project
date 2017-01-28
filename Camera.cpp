#include "Camera.hpp"

Camera::Camera()
{
	this->CameraPosition = this->CameraStartPosition;
	this->CameraDirection = { 0, 0, 1 };
	this->CameraUpDirection = { 0, 1, 0 };
}
Camera::~Camera()
{

}


void Camera::UpdateCamera(
	TCHAR					CharacterMessage,
	POINT					MouseCoordinates,
	GSConstantDataFloats	*FormattedStructData,
	ID3D11Buffer*			*GSConstantBuffer,
	ID3D11DeviceContext*	*DeviceContext
)
{
	bool NeedsToUpdate = false;

	// CAMERA MOVEMENT
	if (CharacterMessage == 'w') {
		NeedsToUpdate = true;
		this->MoveCameraForward();
	}
	else if (CharacterMessage == 'a') {
		NeedsToUpdate = true;
		this->MoveCameraLeft();
	}
	else if (CharacterMessage == 's') {
		NeedsToUpdate = true;
		this->MoveCameraBackward();
	}
	else if (CharacterMessage == 'd') {
		NeedsToUpdate = true;
		this->MoveCameraRight();
	}
	else if (CharacterMessage == 32) {
		NeedsToUpdate = true;
		this->MoveCameraUp();
	}


	// CAMERA ROTATION
	if (MouseCoordinates.y != 0) {
		NeedsToUpdate = true;
		this->RotateCameraVertically(MouseCoordinates);
	}
	if (MouseCoordinates.x != 0) {
		NeedsToUpdate = true;
		this->RotateCameraHorizontally(MouseCoordinates);
	}


	// MISC COMMANDS - Needs to be called after movement and rotation.
	if (CharacterMessage == 'r') {
		NeedsToUpdate = true;
		this->ResetCamera();
	}


	// IF DATA HAS BEEN UPDATED
	if (NeedsToUpdate) {
		// Creates the new viewMatrix
		DirectX::XMMATRIX UnformattedViewMatrix = DirectX::XMMatrixLookToLH(
			this->CameraPosition,
			this->CameraDirection,
			this->CameraUpDirection
		);

		// Updates GSConstantDataFloats with the new viewmatrix.
		DirectX::XMFLOAT4X4 FormattedViewMatrix;
		DirectX::XMStoreFloat4x4(&FormattedViewMatrix, UnformattedViewMatrix);
		FormattedStructData->view = FormattedViewMatrix;

		// Alters the constantbuffer with the updated GSConstDataFloats
		AlterConstantBuffers(*GSConstantBuffer, *FormattedStructData, DeviceContext);
	}
}



// Moves the camera Upwards!
void Camera::MoveCameraUp()
{
	DirectX::XMVECTOR upVector = DirectX::XMVector3Normalize(this->CameraUpDirection);
	upVector = DirectX::XMVectorScale(upVector, this->CameraMovementSpeed);
	this->CameraPosition = DirectX::XMVectorAdd(this->CameraPosition, upVector);
}


// Resets the camera to its original position
void Camera::ResetCamera() {
	this->CameraPosition = this->CameraStartPosition;
	this->CameraDirection = { 0, 0, 1 };
	this->CameraUpDirection = { 0, 1, 0 };
}

// Moves the CameraPosition to the Left, based on the crossproduct of
// the current CameraDirection & CameraUpDirection
void Camera::MoveCameraLeft() {
	DirectX::XMVECTOR MovementVector = DirectX::XMVector3Cross(this->CameraDirection, this->CameraUpDirection);// Creates rightVector
	DirectX::XMVector3Normalize(MovementVector);
	MovementVector = DirectX::XMVectorScale(MovementVector, CameraMovementSpeed);		// The lower the scale, the slower the movement

	this->CameraPosition = DirectX::XMVectorAdd(this->CameraPosition, MovementVector);
}

// Moves the CameraPosition to the Right, based on the crossproduct of
// the current CameraUpDirection & CameraDirection
void Camera::MoveCameraRight() {
	DirectX::XMVECTOR MovementVector = DirectX::XMVector3Cross(this->CameraUpDirection, this->CameraDirection);// Creates rightVector
	DirectX::XMVector3Normalize(MovementVector);
	MovementVector = DirectX::XMVectorScale(MovementVector, CameraMovementSpeed);		// The lower the scale, the slower the movement

	this->CameraPosition = DirectX::XMVectorAdd(this->CameraPosition, MovementVector);
}

// Moves Camera forward based on the current CameraDirection
void Camera::MoveCameraForward() {
	DirectX::XMVECTOR MovementVector = this->CameraDirection;
	DirectX::XMVector3Normalize(MovementVector);
	MovementVector = DirectX::XMVectorScale(MovementVector, CameraMovementSpeed);

	this->CameraPosition = DirectX::XMVectorAdd(this->CameraPosition, MovementVector);
}

// Moves Camera backward based on the current CameraDirection
void Camera::MoveCameraBackward() {
	DirectX::XMVECTOR MovementVector = DirectX::XMVectorScale(this->CameraDirection, -1);
	DirectX::XMVector3Normalize(MovementVector);
	MovementVector = DirectX::XMVectorScale(MovementVector, CameraMovementSpeed);

	this->CameraPosition = DirectX::XMVectorAdd(this->CameraPosition, MovementVector);
}

// Rotates the camera Vertically through Quaternions based
// on the MouseMovement.y
// AIRPLANE MOVEMENT IS TYPICALLY TURNED OFF.
void Camera::RotateCameraVertically(POINT MouseMovement) {
	DirectX::XMVECTOR RightAxis = DirectX::XMVector3Cross(this->CameraUpDirection, this->CameraDirection);

	float RotationAngle = MouseMovement.y * CameraRotationSpeed;

	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis(RightAxis, RotationAngle);

	// Camera-Plane movement is currently turned OFF. Un-comment the line below to turn it ON.
	//this->CameraUpDirection = DirectX::XMVector3Rotate(this->CameraUpDirection, RotationQuaternion);
	this->CameraDirection = DirectX::XMVector3Rotate(this->CameraDirection, RotationQuaternion);
}

// Rotates the camera Horiontally through Quaternions based
// on the MouseMovement.x
void Camera::RotateCameraHorizontally(POINT MouseMovement) {
	float RotationAngle = MouseMovement.x * CameraRotationSpeed;

	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis(this->CameraUpDirection, RotationAngle);

	this->CameraDirection = DirectX::XMVector3Rotate(this->CameraDirection, RotationQuaternion);
}