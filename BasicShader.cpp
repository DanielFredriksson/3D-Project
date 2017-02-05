#include "BasicShader.hpp"

BasicShaderClass::BasicShaderClass()
{
	this->VertexShader = nullptr;
	this->GeometryShader = nullptr;
	this->PixelShader = nullptr;
	this->VertexLayout = nullptr;
	this->BackBufferRTV = nullptr;
	this->DepthStencil = nullptr;
	this->DepthStencilView = nullptr;
}
BasicShaderClass::~BasicShaderClass()
{
	// Currently releasing manually, so nothing needed here atm.
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									PUBLIC FUNCTIONS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void BasicShaderClass::Initialize(ID3D11Device* *Device)
{
	this->InitialiseShaders(Device);
}

void BasicShaderClass::ReleaseAll()
{
	this->VertexShader->Release();
	this->GeometryShader->Release();
	this->PixelShader->Release();
	this->BackBufferRTV->Release();
	this->DepthStencil->Release();
	this->DepthStencilView->Release();
}


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									PRIVATE FUNCTIONS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
ID3D11RenderTargetView* *BasicShaderClass::GetRenderTargetView()
{
	return &this->BackBufferRTV;
}

ID3D11Texture2D* *BasicShaderClass::GetDepthStencil()
{
	return &this->DepthStencil;
}
ID3D11DepthStencilView* *BasicShaderClass::GetDepthStencilView()
{
	return &this->DepthStencilView;
}

void BasicShaderClass::Render(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11Buffer*			*VertexBuffer,
	calcDataClass			*CalcRelatedData
)
{
	static bool hasRun = false;
	this->Clear(DeviceContext, this->DepthStencilView);


	if (!hasRun) {

		this->SetShadersAndShaderResources(DeviceContext);
		this->DefineInputAssembler(
			DeviceContext,
			VertexBuffer
		);

	}
	else {

	}


	UINT32 vertexSize = (sizeof(DirectX::XMFLOAT3) * 2 + sizeof(DirectX::XMFLOAT2));	//Data amount per INDIVIDUAL VERTEX
	UINT32 offset = 0;
	(*DeviceContext)->IASetVertexBuffers(
		0,
		1,
		VertexBuffer,
		&vertexSize,
		&offset
	);
	(*DeviceContext)->Draw((CalcRelatedData->faceCount * 3), 0);

	// DrawIndexed is used when you have an index buffer along side your vertex buffer. However with this
	// system, drawing hard edges is impossible. Instead, by using the Draw functions instead, you can
	// make it so that hard edges appear. This requires that the vectorArray is allocated the following
	// memory: faceCount * 3. You need 1 array with the vertices in order, and then another array where
	// the vertices are loaded in, in the order that the faces present the vertices.
}

void BasicShaderClass::Clear(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11DepthStencilView*	DepthStencilView
)
{
	// clear the back buffer to a deep blue
	float clearColor[] = { 0, 0, 0, 1 };
	(*DeviceContext)->ClearRenderTargetView(this->BackBufferRTV, clearColor);

	// Also clear DepthBuffer via DepthStencilView
	(*DeviceContext)->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}


/* ------------- COMMENTS -------------
Only sets primitive topology and the internal VertexLayout.
*/
void BasicShaderClass::DefineInputAssembler(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11Buffer*			*VertexBuffer
)
{
	UINT32 vertexSize = (sizeof(DirectX::XMFLOAT3) * 2 + sizeof(DirectX::XMFLOAT2));	//Data amount per INDIVIDUAL VERTEX
	UINT32 offset = 0;
	(*DeviceContext)->IASetVertexBuffers(
		0,
		1,
		VertexBuffer,
		&vertexSize,
		&offset
	);
	(*DeviceContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	(*DeviceContext)->IASetInputLayout(this->VertexLayout);
}


void BasicShaderClass::InitialiseShaders(ID3D11Device* *Device)
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

	hr = (*Device)->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->PixelShader);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Pixel Shader'.", L"ERROR: 'CreateShaders()'", MB_OK);

	// we do not need anymore this COM object, so we release it.
	pPS->Release();

}

void BasicShaderClass::SetShadersAndShaderResources(ID3D11DeviceContext* *DeviceContext)
{
	(*DeviceContext)->VSSetShader(this->VertexShader, nullptr, 0);
	(*DeviceContext)->HSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->DSSetShader(nullptr, nullptr, 0);
	(*DeviceContext)->GSSetShader(this->GeometryShader, nullptr, 0);
	(*DeviceContext)->PSSetShader(this->PixelShader, nullptr, 0);
}