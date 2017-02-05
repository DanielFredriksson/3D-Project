#include "DeferredShaderClass.hpp"


DeferredShaderClass::DeferredShaderClass()
{
	this->PixelShader = nullptr;
	this->VertexShader = nullptr;
	this->VertexLayout = nullptr;
	this->SamplerStateWrap = nullptr;
}
DeferredShaderClass::~DeferredShaderClass()
{

}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									PUBLIC FUNCTIONS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


void DeferredShaderClass::InitialiseShaders(ID3D11Device* *Device)
{
	HRESULT hr;

	//----------------------------------- Vertex Shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Def_Vertex.hlsl",
		nullptr,
		nullptr,
		"DEF_VS_main",
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = (*Device)->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->VertexLayout);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Vertex Shader'.", L"ERROR: 'InitialiseShaders() - DeferredShaderClass'", MB_OK);
	pVS->Release();

	
	//----------------------------------- Pixel Shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Def_Pixel.hlsl",
		nullptr,
		nullptr,
		"DEF_PS_main",
		"ps_5_0",
		0,
		0,
		&pPS,
		nullptr
	);
	hr = (*Device)->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->PixelShader);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Pixel Shader'.", L"ERROR: InitialiseShaders() - DeferredShaderClass", MB_OK);
	pPS->Release();


	//----------------------------------- SamplerState
	D3D11_SAMPLER_DESC Sampler_Description;
	Sampler_Description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// Linear interpolation for Minification,Magnification,Mip-level-Sampling.
	Sampler_Description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	// Keep repeating the texture
	Sampler_Description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	// in case of values not between
	Sampler_Description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	// 0-1.
	Sampler_Description.MipLODBias = 0.0f;	// Offset from the calculated MipMap level.
	Sampler_Description.MaxAnisotropy = 1;	// Only relevant if .Filter = ANISOTROPIC|COMPARISON_ANISOTROPIC
	Sampler_Description.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// Comparison will always pass; Difference between source- & destination-data is irrelevant. 
	Sampler_Description.BorderColor[0] = 0;	// BorderColour to be used 
	Sampler_Description.BorderColor[1] = 0;	// if .AdressX is set to 
	Sampler_Description.BorderColor[2] = 0;	// TEXTURE_ADRESS_BORDER
	Sampler_Description.BorderColor[3] = 0;	//
	Sampler_Description.MinLOD = 0;					// Lower end of the mipmap range to clamp access to. (0 is the largest and most detailed mipmaplevel and higher levels are smaller and less detailed)
	Sampler_Description.MaxLOD = D3D11_FLOAT32_MAX;	// Upper end of the mipmap range to clamp access to. (No upper limit = D3D11_FLOAT32_MAX

	hr = (*Device)->CreateSamplerState(&Sampler_Description, &this->SamplerStateWrap);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create SamplerState.", L"ERROR: 'InitialiseShaders() - DeferredShaderClass'", MB_OK);
}


void DeferredShaderClass::ReleaseAll()
{
	this->PixelShader->Release();
	this->VertexShader->Release();
	this->VertexLayout->Release();
	this->SamplerStateWrap->Release();
}



void DeferredShaderClass::SetShadingContext(ID3D11DeviceContext* *DeviceContext)
{
	// Set VertexInputLayout
	(*DeviceContext)->IASetInputLayout(this->VertexLayout);

	// Set Vertex and Pixel shaders that will be used to render
	(*DeviceContext)->VSSetShader(this->VertexShader, nullptr, 0);
	(*DeviceContext)->GSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->PSSetShader(this->PixelShader, nullptr, 0);

	// Set sampler states in the pixel shader
	(*DeviceContext)->PSSetSamplers(0, 1, &this->SamplerStateWrap);
}




void DeferredShaderClass::RenderShader(ID3D11DeviceContext* *DeviceContext, int VerticeCount)
{
	// Render the geometry
	(*DeviceContext)->Draw(VerticeCount, 0);
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									PRIVATE FUNCTIONS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+