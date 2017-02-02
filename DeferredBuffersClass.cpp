#include "DeferredBuffersClass.hpp"

DeferredBuffersClass::DeferredBuffersClass()
{
	for (int i = 0; i < BUFFER_COUNT; i++) {
		this->RenderTargetTextureArray[i] = nullptr;
		this->RenderTargetViewArray[i] = nullptr;
		this->ShaderResourceViewArray[i] = nullptr;
	}
	this->DepthStencilBuffer = nullptr;
	this->DepthStencilView = nullptr;
	this->ViewPort = { 0 };
}
DeferredBuffersClass::~DeferredBuffersClass()
{

}



void DeferredBuffersClass::InitializeBuffers(
	ID3D11Device* *Device,
	UINT TextureWidth,
	UINT TextureHeight)
{
	/*
	The RenderTargetTextureArray's are the containers for the data which the 
	DeferredShaderClass will render to during its internal GPU phase.

	The RenderTargetView's are the lenses with which the CPU handles the data through.

	The ShaderResourceView's are the lenses with which the GPU handles the data through.
	*/

	HRESULT hr;
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//																					//
	//		RenderTargetTexture's,	 RenderTargetView's,	ShaderResourceView's		//
	//																					//
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	// Description for all RenderTargetTexture's
	D3D11_TEXTURE2D_DESC Texture2D_Description;
	ZeroMemory(&Texture2D_Description, sizeof(Texture2D_Description));
	Texture2D_Description.Width = TextureWidth;
	Texture2D_Description.Height = TextureHeight;
	Texture2D_Description.MipLevels = 0;	// '0' generates a full set of subtextures.
	Texture2D_Description.ArraySize = 1;	// We're only using '1' texture.
	Texture2D_Description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// The datatype per pixel, here with four components.
	Texture2D_Description.SampleDesc.Count = 1;	// Multisamples per pixel.
	Texture2D_Description.SampleDesc.Quality = 0;		// Image quality level.
	Texture2D_Description.Usage = D3D11_USAGE_DEFAULT;	// Will only be read/written to by GPU.
	Texture2D_Description.BindFlags = D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;	// This texture will be used as a render target & will be used by shaders
	Texture2D_Description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;	// CPU can read (Despite D3D11_USAGE_DEFAULT), if CPU needs to write, USAGE will have to be changed. [Current same as 0]
	Texture2D_Description.MiscFlags = 0;
	
	// Description for all RenderTargetView's
	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetView_Description;
	ZeroMemory(&RenderTargetView_Description, sizeof(RenderTargetView_Description));
	RenderTargetView_Description.Format = Texture2D_Description.Format;
	RenderTargetView_Description.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetView_Description.Texture2D.MipSlice = 0;
	
	// Description for all ShaderResourceView's
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView_Description;
	ZeroMemory(&ShaderResourceView_Description, sizeof(ShaderResourceView_Description));
	ShaderResourceView_Description.Format = Texture2D_Description.Format;
	ShaderResourceView_Description.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceView_Description.Texture2D.MostDetailedMip = 0;
	ShaderResourceView_Description.Texture2D.MipLevels = 1;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		// Create all Texture2D's
		hr = (*Device)->CreateTexture2D(
			&Texture2D_Description,
			NULL,
			&this->RenderTargetTextureArray[i]
		);
		if (FAILED(hr))
			MessageBox(NULL, L"Failed to Create a Texture2D'.", L"ERROR: 'InitialiseBuffers() - DeferredBuffersClass'", MB_OK);

		// Create all RenderTargetView's
		hr = (*Device)->CreateRenderTargetView(
			this->RenderTargetTextureArray[i],
			&RenderTargetView_Description,
			&this->RenderTargetViewArray[i]
		);
		if (FAILED(hr))
			MessageBox(NULL, L"Failed to Create a RenderTargetView'.", L"ERROR: 'InitialiseBuffers() - DeferredBuffersClass'", MB_OK);

		// Create all ShaderResourceView's
		(*Device)->CreateShaderResourceView(
			this->RenderTargetTextureArray[i],
			&ShaderResourceView_Description,
			&this->ShaderResourceViewArray[i]
		);
		if (FAILED(hr))
			MessageBox(NULL, L"Failed to Create a ShaderResourceView'.", L"ERROR: 'InitialiseBuffers() - DeferredBuffersClass'", MB_OK);
	}

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//																					//
	//					DepthBuffer,	DepthStencilView,	Viewport					//			
	//																					//
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Description for the DepthBuffer
	D3D11_TEXTURE2D_DESC DepthBuffer_Description;
	ZeroMemory(&DepthBuffer_Description, sizeof(DepthBuffer_Description));
	DepthBuffer_Description.Width = TextureWidth;
	DepthBuffer_Description.Height = TextureHeight;
	DepthBuffer_Description.MipLevels = 1;
	DepthBuffer_Description.ArraySize = 1;
	DepthBuffer_Description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBuffer_Description.SampleDesc.Count = 1;
	DepthBuffer_Description.SampleDesc.Quality = 0;
	DepthBuffer_Description.Usage = D3D11_USAGE_DEFAULT;
	DepthBuffer_Description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBuffer_Description.CPUAccessFlags = 0;							// Same as CPU_READ
	DepthBuffer_Description.MiscFlags = 0;
	// Creation of the DepthBuffer
	hr = (*Device)->CreateTexture2D(
		&DepthBuffer_Description,
		NULL,
		&this->DepthStencilBuffer
	);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to Create a DepthBuffer.", L"ERROR: 'InitialiseBuffers() - DeferredBuffersClass'", MB_OK);
	
	
	// Description for the DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilView_Description;
	ZeroMemory(&DepthStencilView_Description, sizeof(DepthStencilView_Description));
	DepthStencilView_Description.Format = DepthBuffer_Description.Format;
	DepthStencilView_Description.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilView_Description.Texture2D.MipSlice = 0;
	// Creation of the DepthStencilView
	hr = (*Device)->CreateDepthStencilView(
		this->DepthStencilBuffer,
		&DepthStencilView_Description,
		&this->DepthStencilView
	);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to Create a DepthStencilView'.", L"ERROR: 'InitialiseBuffers() - DeferredBuffersClass'", MB_OK);

	// Setting up the viewport
	this->ViewPort.Width = float(TextureWidth);
	this->ViewPort.Height = float(TextureHeight);
	this->ViewPort.MaxDepth = 1.0f;
	this->ViewPort.MinDepth = 0.0f;
	this->ViewPort.TopLeftX = 0.0f;
	this->ViewPort.TopLeftY = 0.0f;
}

void DeferredBuffersClass::SetAllRenderTargets(ID3D11DeviceContext* *DeviceContext)
{
	(*DeviceContext)->OMSetRenderTargets(
		BUFFER_COUNT,
		this->RenderTargetViewArray,
		this->DepthStencilView
	);

	(*DeviceContext)->RSSetViewports(1, &this->ViewPort);
}

void DeferredBuffersClass::ClearAllRenderTargets(
	ID3D11DeviceContext*	*DeviceContext,
	float					ClearColor_Red,
	float					ClearColor_Blue,
	float					ClearColor_Green,
	float					ClearColor_Alpha
)
{
	/*
	Clears all RenderTarget's.
	Clears the DepthStencilView.
	*/

	// Sets up the background color
	float colour[4];
	colour[1] = ClearColor_Red;
	colour[2] = ClearColor_Blue;
	colour[3] = ClearColor_Green;
	colour[4] = ClearColor_Alpha;

	// Clears all RenderTarget's
	for (int i = 0; i < BUFFER_COUNT; i++) {
		(*DeviceContext)->ClearRenderTargetView(this->RenderTargetViewArray[i], colour);
	}

	// Clears the DepthStencilView.
	(*DeviceContext)->ClearDepthStencilView(
		this->DepthStencilView,
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
	);
}

void DeferredBuffersClass::ReleaseAll()
{
	/*
	Clears every 'private' variable in the class.
	*/
	for (int i = 0; i < BUFFER_COUNT; i++) {
		this->RenderTargetTextureArray[i]->Release();
		this->RenderTargetTextureArray[i] = nullptr;

		this->RenderTargetViewArray[i]->Release();
		this->RenderTargetViewArray[i] = nullptr;

		this->ShaderResourceViewArray[i]->Release();
		this->ShaderResourceViewArray[i] = nullptr;
	}
	this->DepthStencilBuffer->Release();
	this->DepthStencilBuffer = nullptr;
	
	this->DepthStencilView->Release();
	this->DepthStencilView = nullptr;

	this->ViewPort = { 0 };
}
