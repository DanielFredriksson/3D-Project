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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		MessageBox(NULL, L"Failed to create 'Pixel Shader'.", L"ERROR: InitialiseShaders() - DeferredShaderClass", MB_OK);

	// we do not need anymore this COM object, so we release it.
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

	
	//----------------------------------- Matrix Constant Buffer
	D3D11_BUFFER_DESC MatrixBuffer_Description;
	MatrixBuffer_Description.Usage = D3D11_USAGE_DYNAMIC;	// Can be Written-to by the CPU and read-by the GPU
	MatrixBuffer_Description.ByteWidth = sizeof(MatrixBufferStored);	// Bytesize.
	MatrixBuffer_Description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// [What kind of buffer?]
	MatrixBuffer_Description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// In accordance to D3D_USAGE_DYNAMIC.
	MatrixBuffer_Description.MiscFlags = 0;				// Extra-Fancy settings.
	MatrixBuffer_Description.StructureByteStride = 0;	// Only relevant if it's a 'Structured Buffer' ( A buffer that contains elements of equal sizes )

	hr = (*Device)->CreateBuffer(&MatrixBuffer_Description, NULL, &this->MatrixBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create MatrixBuffer.", L"ERROR: 'InitialiseShaders() - DeferredShaderClass'", MB_OK);


}


void DeferredShaderClass::ReleaseAll()
{
	this->PixelShader->Release();
	this->VertexShader->Release();
	this->VertexLayout->Release();
	this->SamplerStateWrap->Release();
	this->MatrixBuffer->Release();
}


void DeferredShaderClass::SetShaderParameters(
	ID3D11DeviceContext*		*DeviceContext,
	ID3D11ShaderResourceView*	*texture,
	DirectX::XMFLOAT4X4			FormattedWorldMatrix,
	DirectX::XMFLOAT4X4			FormattedViewMatrix,
	DirectX::XMFLOAT4X4			FormattedProjectionMatrix
)
{
	// Map the constant buffer so it can be written to.
	MatrixBufferStored *DataPointer;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HRESULT hr = (*DeviceContext)->Map(this->MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to Map.", L"ERROR: 'SetShaderParameters()' - DeferredShaderClass", MB_OK);
	DataPointer = (MatrixBufferStored*)MappedResource.pData;

	// Do shit.
	DataPointer->world = FormattedWorldMatrix;
	DataPointer->view = FormattedViewMatrix;
	DataPointer->projection = FormattedProjectionMatrix;

	// Unmap
	(*DeviceContext)->Unmap(this->MatrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader
	int BufferNumber = 0;

	(*DeviceContext)->VSSetConstantBuffers(BufferNumber, 1, &this->MatrixBuffer);

	(*DeviceContext)->PSSetShaderResources(0, 1, texture);
	// Set the constant buffer in the vertex shader with the updated values.
}


void DeferredShaderClass::RenderShader(ID3D11DeviceContext* *DeviceContext, int VerticeCount)
{
	// Set VertexInputLayout
	(*DeviceContext)->IASetInputLayout(this->VertexLayout);

	// Set Vertex and Pixel shaders that will be used to render
	(*DeviceContext)->VSSetShader(this->VertexShader, NULL, 0);
	(*DeviceContext)->PSSetShader(this->PixelShader, NULL, 0);

	// Set sampler states in the pixel shader
	(*DeviceContext)->PSSetSamplers(0, 1, &this->SamplerStateWrap);

	// Render the geometry
	(*DeviceContext)->Draw(VerticeCount, 0);
}


void DeferredShaderClass::Render(
	ID3D11DeviceContext*		*DeviceContext,
	ID3D11ShaderResourceView*	*texture,
	DirectX::XMFLOAT4X4			FormattedWorldMatrix,
	DirectX::XMFLOAT4X4			FormattedViewMatrix,
	DirectX::XMFLOAT4X4			FormattedProjectionMatrix,
	int							VerticeCount
)
{
	this->SetShaderParameters(
		DeviceContext,
		texture,
		FormattedWorldMatrix,
		FormattedViewMatrix,
		FormattedProjectionMatrix
	);

	this->RenderShader(DeviceContext, VerticeCount);
}