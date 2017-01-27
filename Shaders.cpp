#include "Shaders.hpp"

void CreateShadersFunc(
	ID3D11Device*			Device,
	ID3D11VertexShader*		*VertexShader,
	ID3D11GeometryShader*	*GeometryShader,
	ID3D11PixelShader*		*PixelShader,
	ID3D11InputLayout* *	VertexLayout
)
{
	HRESULT hr;

	//C R E A T E     V E R T E X     S H A D E R
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	Device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, VertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = Device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), VertexLayout);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Vertex Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//----------------------------------------------------------------------------------------------------------------------------------

	//C R E A T E     G E O M E T R Y     S H A D E R
	ID3DBlob* pGS = nullptr;
	ID3DBlob* error_GS = nullptr;
	D3DCompileFromFile(
		L"Geometry.hlsl",	//Filename
		nullptr,			//Optional Macros
		nullptr,			//Optional Include Files
		"GS_main",			//Entry Point
		"gs_5_0",			//Geometry Model (target)
		0,					//Geometry Compile Options
		0,					//Effect Compile Options
		&pGS,				//Double Pointer to ID3DBlob
		&error_GS			//Pointer for Error Blob messages.
	);

	if (error_GS)
	{
		OutputDebugStringA((char*)error_GS->GetBufferPointer());
		error_GS->Release();
	}

	hr = Device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(),
		nullptr, GeometryShader);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Geometry Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	//We no longer need this COM object and therefore release it
	pGS->Release();

	//----------------------------------------------------------------------------------------------------------------------------------


	//C R E A T E     P I X E L     S H A D E R
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Pixel.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr = Device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, PixelShader);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Pixel Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pPS->Release();
}

// Currently:
//
void SetShadersAndShaderResources(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11VertexShader*		VertexShader,
	ID3D11GeometryShader*	GeometryShader,
	ID3D11PixelShader*		PixelShader
)
{
	(*DeviceContext)->VSSetShader(VertexShader, nullptr, 0);
	(*DeviceContext)->HSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->DSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->GSSetShader(GeometryShader, nullptr, 0);
	(*DeviceContext)->PSSetShader(PixelShader, nullptr, 0);
}