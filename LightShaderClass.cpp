#include "LightShaderClass.hpp"



LightShaderClass::LightShaderClass()
{
	this->VertexLayout = nullptr;
	this->VertexShader = nullptr;
	this->GeometryShader = nullptr;
	this->PixelShader = nullptr;
	this->BackBufferRTV = nullptr;
}
LightShaderClass::~LightShaderClass()
{

}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									PUBLIC FUNCTIONS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


void LightShaderClass::InitializeShaders(ID3D11Device* *Device)
{
	HRESULT hr;
	//C R E A T E     V E R T E X     S H A D E R
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Light_Vertex.hlsl",
		nullptr,
		nullptr,
		"LIGHT_VS_main",
		"vs_5_0",
		0,
		0,
		&pVS,
		nullptr
	);
	(*Device)->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->VertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = (*Device)->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->VertexLayout);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Vertex Shader'.", L"ERROR: 'InitialiseShaders() - DeferredShaderClass'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();


	//----------------------------------------------------------------------------------------------------------------------------------
	//C R E A T E     G E O M E T R Y     S H A D E R
	ID3DBlob* pGS = nullptr;
	ID3DBlob* error_GS = nullptr;
	D3DCompileFromFile(
		L"Light_Geometry.hlsl",
		nullptr,
		nullptr,
		"LIGHT_GS_main",
		"gs_5_0",
		0,
		0,
		&pGS,
		&error_GS
	);

	if (error_GS)
	{
		OutputDebugStringA((char*)error_GS->GetBufferPointer());
		error_GS->Release();
	}

	hr = (*Device)->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(),
		nullptr, &this->GeometryShader);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Geometry Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	//We no longer need this COM object and therefore release it
	pGS->Release();

	//----------------------------------------------------------------------------------------------------------------------------------
	//C R E A T E     P I X E L     S H A D E R
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Light_Pixel.hlsl",
		nullptr,
		nullptr,
		"LIGHT_PS_main",
		"ps_5_0",
		0,
		0,
		&pPS,
		nullptr
	);

	hr = (*Device)->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->PixelShader);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Pixel Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pPS->Release();

}

void LightShaderClass::SetShadersAndShaderResources(ID3D11DeviceContext* *DeviceContext)
{
	(*DeviceContext)->VSSetShader(this->VertexShader, nullptr, 0);
	(*DeviceContext)->HSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->DSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->GSSetShader(this->GeometryShader, nullptr, 0);
	(*DeviceContext)->PSSetShader(this->PixelShader, nullptr, 0);

	// Also sets ShaderResourceView's according to the Texture2D's
	//
	//
}

void LightShaderClass::ReleaseAll()
{
	this->VertexLayout->Release();
	this->VertexShader->Release();
	this->GeometryShader->Release();
	this->PixelShader->Release();
	this->BackBufferRTV->Release();
}





//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									PRIVATE FUNCTIONS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+