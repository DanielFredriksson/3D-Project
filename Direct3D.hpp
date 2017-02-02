#ifndef DIRECT3D_HPP
#define DIRECT3D_HPP

#include <d3d11.h>
#include "Globals.hpp"	// InitialiseDepthStencilAndView(), CreateViewport()

class Direct3DContext {
private:
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

public:
	Direct3DContext();
	~Direct3DContext();

	// TO-BE PRIVATE BELOW:


	// Calls InitialiseDepthStencilAndView()
	void CreateDirect3DContext(
		HWND	wndHandle,
		ID3D11RenderTargetView*	*BackBufferRTV,
		ID3D11Texture2D*		*DepthStencil,		
		ID3D11DepthStencilView*	*DepthStencilView
	);
	void InitialiseDepthStencilAndView(
		ID3D11Texture2D*		*DepthStencil,
		ID3D11DepthStencilView* *DepthStencilView
	);

	/* ------------- COMMENTS -------------
	Creates a viewport according to global SCREEN_WIDTH/HEIGHT and sets
	it to the rasterizer stage.
	*/
	void CreateSetViewport();



	// TO-BE PUBLIC BELOW:

	ID3D11Device* *GetDevice();
	ID3D11DeviceContext* *GetDeviceContext();
	IDXGISwapChain* *GetSwapChain();
	// get swapchain


	void Initialize(
		HWND	wndHandle,
		ID3D11RenderTargetView*	*BackBufferRTV,
		ID3D11Texture2D*		*DepthStencil,
		ID3D11DepthStencilView*	*DepthStencilView
	);


	void ReleaseAll();


};







#endif