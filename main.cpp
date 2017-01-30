






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

// --- CURRENTLY TESTING
#include "DeferredBuffersClass.hpp"
#include "DeferredShaderClass.hpp"

// --- CURRENTLY TESTING

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
		MatrixBufferLoaded UnformattedMatrixData;						// Unformatted, XMVECTOR/XMMATRIX types
		MatrixBufferStored FormattedMatrixData;							// Formatted,	XMFLOATS

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

		// Initialise CB-Matrices, Reformat them, and set the reformatted data to a Created&Set constant buffer
		InitializeConstantMatrices(
			&UnformattedMatrixData
		);
		MatrixToFloat4X4Reformat(	//~ ConstantBuffers
			&UnformattedMatrixData,
			FormattedMatrixData
		);
		CreateSetConstantBuffers(	//~ ConstantBuffers
			&FormattedMatrixData,
			&DirectXManager.Device,
			&DirectXManager.DeviceContext,
			&gConstantBuffer
		);
		//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		//																					//
		//									TESTING AREA									//
		//																					//
		//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		
		DeferredBuffersClass TestBuffers;
		/* Currently no error with:
		TestBuffers.InitializeBuffers(&DirectXManager.Device, SCREEN_WIDTH, SCREEN_HEIGHT);
		TestBuffers.SetAllRenderTargets(&DirectXManager.DeviceContext);
		
		*/
		/* Currently no error with: 
		TestShader.InitialiseShaders(&DirectXManager.Device);
		*/
		DeferredShaderClass TestShader;
		



		//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		//																					//
		//									TESTING AREA									//
		//																					//
		//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

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
					&FormattedMatrixData,
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
