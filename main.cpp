






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

// APPLICATION
#include "ApplicationClass.hpp"

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

	
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//																					//
	//									IMPROVED MAIN									//
	//																					//
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// When this works:
	//	- Remove '1' from the ConstantBufferLoaded1 & ConstantBufferStored1
	//	- Remove '1' from AlterConstantBuffers
	//
	//

	ApplicationClass Application;

	Application.Initialize(hInstance);

	Application.SetObjectData();

	ShowWindow(Application.GetWNDHandle(), nCmdShow);

	while (Application.GetInputMessage()->message != WM_QUIT)	// Needs to de-reference the message? (prolly not?)
	{

		if (PeekMessage(Application.GetInputMessage(), nullptr, 0, 0, PM_REMOVE))
		{


			TranslateMessage(Application.GetInputMessage());	// Calls WindowProcedure in some part of its internal process.
			DispatchMessage(Application.GetInputMessage());


			Application.UpdateCamera();


		}
		else
		{

			Application.Render();

			Application.Present();

		}
	}

	Application.Shutdown();

	return (int)(*Application.GetInputMessage()).wParam;
}
