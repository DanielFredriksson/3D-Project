#ifndef DIRECT3D_HPP
#define DIRECT3D_HPP

#include <d3d11.h>
#include "Globals.hpp"	// InitialiseDepthStencilAndView(), CreateViewport()

class Direct3DContext {
private:


public:
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

	Direct3DContext();
	~Direct3DContext();

	void CreateDirect3DContext(
		HWND	wndHandle,
		ID3D11RenderTargetView*	*BackBufferRTV,
		ID3D11Texture2D*		*DepthStencil,		// Calls InitialiseDepthStencilAndView()
		ID3D11DepthStencilView*	*DepthStencilView	//
	);
	void InitialiseDepthStencilAndView(
		ID3D11Texture2D*		*DepthStencil,
		ID3D11DepthStencilView* *DepthStencilView
	);


	void CreateViewport();




	void Release();


};







#endif