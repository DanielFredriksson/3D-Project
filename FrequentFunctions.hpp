#ifndef DYNAMICFUNCTIONS_HPP
#define DYNAMICFUNCTIONS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>
#include "Shaders.hpp"
#include "Initializations.hpp"
#include "Globals.hpp"

LRESULT CALLBACK WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Clear(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11RenderTargetView* BackBufferRTV,
	ID3D11DepthStencilView*	DepthStencilView
);

void Render(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11RenderTargetView*	BackBufferRTV,
	ID3D11Buffer*			*VertexBuffer,
	ID3D11Buffer*			*IndexBuffer,
	calcData				*CalcRelatedData,
	ID3D11InputLayout*		VertexLayout,
	ID3D11VertexShader*		VertexShader,
	ID3D11GeometryShader*	GeometryShader,
	ID3D11PixelShader*		PixelShader,		
	ID3D11DepthStencilView*	DepthStencilView	// Needs to be cleared
);


#endif