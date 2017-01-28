#ifndef BASICSHADER_HPP
#define BASICSHADER_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

#include "FrequentFunctions.hpp"	// Currently here for Clear()
#include "Globals.hpp"				// Currently here for VERTICE_COUNT_TRIANGLES in Render();
#include "objLoader.hpp"



class BasicShaderClass {
private:
	ID3D11InputLayout*		VertexLayout;
	ID3D11VertexShader*		VertexShader;
	ID3D11GeometryShader*	GeometryShader;
	ID3D11PixelShader*		PixelShader;

	ID3D11Texture2D*		DepthStencil;		// These has to be initialized by Direct3DContext.CreateDirect3DContext
	ID3D11DepthStencilView*	DepthStencilView;	//

	ID3D11RenderTargetView*	BackBufferRTV;

public:
	BasicShaderClass();
	~BasicShaderClass();

	ID3D11RenderTargetView* *GetRenderTargetView();

	ID3D11Texture2D*		*GetDepthStencil();
	ID3D11DepthStencilView*	*GetDepthStencilView();

	void Render(
		ID3D11DeviceContext*	*DeviceContext,
		ID3D11Buffer*			*VertexBuffer,
		ID3D11Buffer*			*IndexBuffer,
		calcData				*CalcRelatedData
	);
	void Clear(
		ID3D11DeviceContext*	*DeviceContext,
		ID3D11DepthStencilView*	DepthStencilView
	);

	void InitialiseShaders(ID3D11Device* *Device);

	void SetShadersAndShaderResources(ID3D11DeviceContext* *DeviceContext);
	void DefineInputAssembler(
		ID3D11DeviceContext*	*DeviceContext,				// Is affected by a difference in VertexSize.
		ID3D11Buffer*			*VertexBuffer,
		ID3D11Buffer*			*IndexBuffer
	);

	void ReleaseAll();

};


#endif