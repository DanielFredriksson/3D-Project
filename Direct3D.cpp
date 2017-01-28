#include "Direct3D.hpp"

Direct3DContext::Direct3DContext()
{
	this->SwapChain = nullptr;
	this->Device = nullptr;
	this->DeviceContext = nullptr;
}
Direct3DContext::~Direct3DContext()
{

}

void Direct3DContext::CreateDirect3DContext(
	HWND	wndHandle,
	ID3D11RenderTargetView*	*BackBufferRTV,
	ID3D11Texture2D*		*DepthStencil,
	ID3D11DepthStencilView*	*DepthStencilView
)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->SwapChain,
		&this->Device,
		NULL,
		&this->DeviceContext);

	// ~ DepthBuffer
	InitialiseDepthStencilAndView(DepthStencil, DepthStencilView);  // Has to be called after we've created Device,DeviceContext and SwapChain // Has to be called before we set render targets
																	//
	if (SUCCEEDED(hr))
	{

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		this->Device->CreateRenderTargetView(pBackBuffer, NULL, BackBufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		this->DeviceContext->OMSetRenderTargets(1, BackBufferRTV, *DepthStencilView);

	}
}


void Direct3DContext::CreateViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->DeviceContext->RSSetViewports(1, &vp);
}

void Direct3DContext::InitialiseDepthStencilAndView(
	ID3D11Texture2D*		*DepthStencil,
	ID3D11DepthStencilView* *DepthStencilView)
{
	D3D11_TEXTURE2D_DESC DepthDesc;
	memset(&DepthDesc, 0, sizeof(DepthDesc));
	DepthDesc.Width = SCREEN_WIDTH;
	DepthDesc.Height = SCREEN_HEIGHT;
	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;
	DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;			// D32 = Depth - 32byte | 24 needed for depth, 8 needed for stencil   | D24_UNORM_S8_UINT
	DepthDesc.SampleDesc.Count = 4;						// Count and quality need to be the same values which are used,
	DepthDesc.SampleDesc.Quality = 0;					// "If multi-sample antialiasing is being used, all bound render targets and depth buffers must have the same sample counts and quality levels."
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.CPUAccessFlags = 0;
	DepthDesc.MiscFlags = 0;

	// No initial data since data is to be fetched - Initdata is therefore NULL
	HRESULT hr = this->Device->CreateTexture2D(&DepthDesc, NULL, DepthStencil);
	if (FAILED(hr))
		MessageBox(NULL, L"Falied to create DepthStencil", L"CreateDepthStencil()", MB_OK);

	// Default DepthStencilView - Desc is therefore NULL
	hr = this->Device->CreateDepthStencilView(*DepthStencil, NULL, DepthStencilView);
	if (FAILED(hr))
		MessageBox(NULL, L"Falied to create DepthStencilView", L"CreateDepthStencil()", MB_OK);

	// DepthStencilState is default since we're not implementing it.
}

void Direct3DContext::Release()
{
	this->SwapChain->Release();
	this->Device->Release();
	this->DeviceContext->Release();
}