#ifndef INITIALIZATIONS_HPP
#define INITIALIZATIONS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>
#include "Globals.hpp"
#include <DirectXMath.h>
#include "DepthBuffer.hpp"
#include "objLoader.hpp"

HRESULT CreateDirect3DContext(
	HWND wndHandle,
	IDXGISwapChain*			*SwapChain,
	ID3D11Device*			*Device,
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11RenderTargetView*	*BackBufferRTV,
	ID3D11Texture2D*		*DepthStencil,
	ID3D11DepthStencilView*	*DepthStencilView
);

void CreateViewport(ID3D11DeviceContext* *DeviceContext);

HWND InitiateWindow(
	HINSTANCE hInstance,
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
);

void DefineInputAssembler(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11Buffer*			*VertexBuffer,
	ID3D11Buffer*			*IndexBuffer,
	ID3D11InputLayout*		VertexLayout
);

void CreateObjectData(
	ID3D11Device* *Device,
	ID3D11Buffer* *VertexBuffer,
	ID3D11Buffer* *IndexBuffer,
	objData* *object,
	calcData *data
);


#endif