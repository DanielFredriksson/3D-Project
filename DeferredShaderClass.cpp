#include "DeferredShaderClass.hpp"


DeferredShaderClass::DeferredShaderClass()
{
	this->PixelShader = nullptr;
	this->VertexShader = nullptr;
	this->VertexLayout = nullptr;
	this->SamplerStateWrap = nullptr;
	this->MatrixBuffer = nullptr;
}
DeferredShaderClass::~DeferredShaderClass()
{

}



void DeferredShaderClass::InitialiseShaders(ID3D11Device* *Device)
{
	HRESULT hr;
	//C R E A T E     V E R T E X     S H A D E R
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Def_Vertex.hlsl",
		nullptr,
		nullptr,
		"DEF_VS_main",		// entry point
		"vs_5_0",			// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	(*Device)->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->VertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = (*Device)->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->VertexLayout);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Vertex Shader'.", L"ERROR: 'InitialiseShaders() - DeferredShaderClass'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//----------------------------------------------------------------------------------------------------------------------------------
	//C R E A T E     P I X E L     S H A D E R
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Def_Pixel.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"DEF_PS_main",	// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr = (*Device)->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->PixelShader);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Pixel Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pPS->Release();
}

void DeferredShaderClass::ReleaseAll()
{
	this->PixelShader->Release();
	this->VertexShader->Release();
	this->VertexLayout->Release();
	this->SamplerStateWrap->Release();
	this->MatrixBuffer->Release();
}