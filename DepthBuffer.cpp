#include "DepthBuffer.hpp"

void InitialiseDepthStencilAndView(
	ID3D11Device*			*Device,
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
	HRESULT hr = (*Device)->CreateTexture2D(&DepthDesc, NULL, DepthStencil);
	if (FAILED(hr))
		MessageBox(NULL, L"Falied to create DepthStencil", L"CreateDepthStencil()", MB_OK);

	// Default DepthStencilView - Desc is therefore NULL
	hr = (*Device)->CreateDepthStencilView(*DepthStencil, NULL, DepthStencilView);
	if (FAILED(hr))
		MessageBox(NULL, L"Falied to create DepthStencilView", L"CreateDepthStencil()", MB_OK);

	// DepthStencilState is default since we're not implementing it.
}