


// Make it so TriangleVertices can be send as an in-parameter to CreateTriangleData,
// preferably declaring it in "objects.hpp"
//
// Send halfXYZ to the geometryShader through the constant buffer.
//
// Make the room bigger by making the inital vertices bigger.
//
//
//
//









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

//
#include "Shaders.hpp"
#include "Initializations.hpp"		// Also includes "Objects.hpp"
#include "ConstantBuffers.hpp"
#include "FrequentFunctions.hpp"
#include "objLoader.hpp"

//
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

// FOR MEMORY LEAKS
#include <crtdbg.h>


HRESULT ghr;

// DIRECTX REQUIRED
IDXGISwapChain* gSwapChain = nullptr;
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;

ID3D11RenderTargetView* gBackBufferRTV = nullptr;

ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11Buffer* gIndexBuffer = nullptr;
ID3D11Buffer* gConstantBuffer = nullptr;

// DEPTH BUFFER
ID3D11Texture2D* gDepthStencil = nullptr;
ID3D11DepthStencilView* gDepthStencilView = nullptr;

// INPUT ASSEMBLY
ID3D11InputLayout* gVertexLayout = nullptr;

// PIPELINE STAGES
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;




int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MSG msg = { 0 };
	HWND wndHandle = InitiateWindow(hInstance, WindowsProcedure); //1. Create Window
	
	if (wndHandle)
	{
		//~ Initializations
		CreateDirect3DContext(
			wndHandle,
			&gSwapChain,
			&gDevice,
			&gDeviceContext,
			&gBackBufferRTV,
			&gDepthStencil,
			&gDepthStencilView);

		//~ Initializations
		CreateViewport(&gDeviceContext);

		//~ Shaders
		CreateShadersFunc(gDevice, &gVertexShader, &gGeometryShader, &gPixelShader, &gVertexLayout);	//4. Create Vertex and Pixel Shaders

		// VARIOUS OBJECTS
		std::string test_string;
		test_string = "obj_files/teddy.obj";	// Requires a cam of -35 (z-direction)
		//test_string = "obj_files/cube.obj";	// Requires a cam of -2 (z-direction)

		//~ ObjectLoader
		objLoader firstObject;

		firstObject.importObjFile(test_string);

		//~ Initializations
		CreateObjectData(&gDevice,
			&gVertexBuffer,
			&gIndexBuffer,
			firstObject.get_objData(),
			firstObject.get_calcData()
		);

		GSConstantDataMatrices GSConstDataMatrices;			// Create ONE 'GSConstantDataMatrices'. This holds various different
															// 'XMMATRIX' objects; world, view, projection, flip, etc.
		//~ ConstantBuffers
		InitializeConstantMatrices(&GSConstDataMatrices);	// Fill the three above mentioned 'XMMATRIX's with information

		GSConstantDataFloats GSConstDataFloats;				

		//~ ConstantBuffers
		MatrixToFloat4X4Reformat(&GSConstDataMatrices, GSConstDataFloats);  // Reformat the 'XMMATRIX's to 'XMFLOAT4X4's so the HLSL can work
																			// with the data.
		//~ ConstantBuffers
		CreateSetConstantBuffers(	// Create Constant Buffer & Send to Geometry Shader
			&GSConstDataFloats,
			&gDevice,
			&gDeviceContext,
			&gConstantBuffer
		);

		ShowWindow(wndHandle, nCmdShow);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//~ FrequentFunctions
				Render(		//8. Render
					&gDeviceContext,
					gBackBufferRTV,
					&gVertexBuffer,
					&gIndexBuffer,
					&CalcRelatedData_teddy,
					gVertexLayout,
					gVertexShader,
					gGeometryShader,
					gPixelShader,
					gDepthStencilView
				); 

				gSwapChain->Present(1, 0); //9. Swap front- och back-buffer
			}
		}

		gVertexBuffer->Release();
		gIndexBuffer->Release();	/// Releasing the Index Buffer
		gConstantBuffer->Release();	/// Releasing the Constant Buffer

		gVertexLayout->Release();
		gVertexShader->Release();
		gGeometryShader->Release();
		gPixelShader->Release();

		gBackBufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		DestroyWindow(wndHandle);
	}

	return (int) msg.wParam;
}
