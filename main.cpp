






//--------------------------------------------------------------------------------------
//	DirectX 11 Starting Template Created by Stefan Petersson (BTH) 2014
//	Project Coded by Daniel Fredriksson & Oliver Glandberger
//--------------------------------------------------------------------------------------

//
#include <windows.h>
#include "Globals.hpp"
#include "Initializations.hpp"

//
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Initializations.hpp"		// Also includes "Objects.hpp"
#include "ConstantBuffers.hpp"
#include "FrequentFunctions.hpp"

// INPUT
#include "objLoader.hpp"

// DIRECTX3D
#include "Direct3D.hpp"

// DEFERRED-SHADER
#include "BasicShader.hpp"

// CAMERA
#include "Camera.hpp"

//
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

// FOR MEMORY LEAKS
#include <crtdbg.h>


// INPUT ASSEMBLY
ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11Buffer* gIndexBuffer = nullptr;

// CONSTANT BUFFER
ID3D11Buffer* gConstantBuffer = nullptr;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Direct3DContext DirectXManager;
	Camera MainCam;
	BasicShaderClass BasicShader;

	MSG msg = { 0 };													// Necessary to handle input to WindowsProcedure
	HWND wndHandle = InitiateWindow(hInstance, WindowsProcedure);		// Necessary to use a window

	if (wndHandle)
	{
		// Containers which will be send to the GeometryShader via Constant Buffer
		GSConstantDataMatrices GSConstDataMatrices;						// Unformatted, XMVECTOR/XMMATRIX types
		GSConstantDataFloats GSConstDataFloats;							// Formatted,	XMFLOATS

		BasicShader.GetRenderTargetView();

		// Set up 3DContext & viewport
		DirectXManager.CreateDirect3DContext(
			wndHandle,
			BasicShader.GetRenderTargetView(),
			BasicShader.GetDepthStencil(),
			BasicShader.GetDepthStencilView()
		);
		DirectXManager.CreateViewport();

		// Initialise 
		BasicShader.InitialiseShaders(&DirectXManager.Device);

		// VARIOUS OBJECTS
		std::string test_string;
		test_string = "obj_files/teddy.obj";	// Requires a cam of -35 (z-direction)

		//~ ObjectLoader
		objLoader firstObject;

		firstObject.importObjFile(test_string);

		//~ Initializations
		CreateObjectData(
			&DirectXManager.Device,
			&gVertexBuffer,
			&gIndexBuffer,
			firstObject.get_objData(),
			firstObject.get_calcData()
		);

		////------------------------------------ WILL BE REPLACED WITH OLIVER CODE ------------------------------------
		//TriangleVertex TriangleVertices[VERTICE_COUNT_TRIANGLES] =
		//{
		//	/* WHEN CHANGING, ALSO CHANGE:
		//	verticeSize...

		//	*/

		//	-0.5f, -0.5f, 0.0f,		//v0 pos (Bottom Left)
		//	1.0f, 0.0f, 0.0f,		//v0 colour

		//	-0.5f, 0.5f, 0.0f,		//v1 pos (Top Left)
		//	0.0f, 1.0f, 0.0f,		//v1 colour

		//	0.5f, -0.5f, 0.0f,		//v2 pos (Bottom Right)
		//	0.0f, 0.0f, 1.0f,		//v2 colour

		//	0.5f, 0.5f, 0.0f,		//v3 pos (Top Right)
		//	1.0f, 0.0f, 1.0f		//v3 colour
		//};
		////~ Initializations
		//CreateTriangleData(DirectXManager.Device, gVertexBuffer, TriangleVertices);
		//------------------------------------ WILL BE REPLACED WITH OLIVER CODE ------------------------------------


		// Initialise CB-Matrices, Reformat them, and set the reformatted data to a Created&Set constant buffer
		InitializeConstantMatrices(
			&GSConstDataMatrices
		);
		MatrixToFloat4X4Reformat(	//~ ConstantBuffers
			&GSConstDataMatrices,
			GSConstDataFloats
		);
		CreateSetConstantBuffers(	//~ ConstantBuffers
			&GSConstDataFloats,
			&DirectXManager.Device,
			&DirectXManager.DeviceContext,
			&gConstantBuffer
		);

		// Display window
		ShowWindow(wndHandle, nCmdShow);


		POINT LastMouseCoordinates = { 0, 0 };		// Gets the value from the last mouse coordinates
		POINT CursorMovement = { 0, 0 };			// Gets the difference between New&Last coordinates
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);	// Calls WindowProcedure in some part of its internal process.
				DispatchMessage(&msg);


				CursorMovement.x = msg.pt.x - LastMouseCoordinates.x;
				CursorMovement.y = msg.pt.y - LastMouseCoordinates.y;
				LastMouseCoordinates = msg.pt;

				MainCam.UpdateCamera(		// Updates the camera dependant on the character input data.
					msg.wParam,				// msg.Wparam Contains the character of the key which was pressed.
					CursorMovement,
					&GSConstDataFloats,
					&gConstantBuffer,
					&DirectXManager.DeviceContext
				);


			}
			else
			{
				// Render
				BasicShader.Render(
					&DirectXManager.DeviceContext,
					&gVertexBuffer,
					&gIndexBuffer,
					firstObject.get_calcData()
				);

				// Swap front(window)-buffer & back-buffer
				DirectXManager.SwapChain->Present(1, 0);
			}
		}

		gVertexBuffer->Release();
		gConstantBuffer->Release();	///Releasing the Constant Buffer

		BasicShader.ReleaseAll();
		DirectXManager.Release();

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}
