#ifndef BASICSHADER_HPP
#define BASICSHADER_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

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
	ID3D11SamplerState*		SamplerState;

	ID3D11RenderTargetView*	BackBufferRTV;

public:
	BasicShaderClass();
	~BasicShaderClass();

	// TO BE PRIVATE BELOW:

	/* ------------- COMMENTS -------------
	OBS: This function is supposed to be called per object!	
	This function sets the VertexBuffer parameter to the Input Assembly with 
	the vertexsize (float3*2 + float2) and the offset 0.
	This function sets shaders and shader resources the first time it's called.
	[Does not clean]
	*/
	void Render(
		ID3D11DeviceContext*		*DeviceContext,
		ID3D11Buffer*				*VertexBuffer,
		ObjectDataClass				*ObjectData,
		ID3D11ShaderResourceView*	*ShaderResourceView
	);

	void Clear(
		ID3D11DeviceContext*	*DeviceContext,
		ID3D11DepthStencilView*	DepthStencilView
	);


	void InitialiseShaders(ID3D11Device* *Device);
	void InitializeSamplerState(ID3D11Device* *Device);


	void SetShadersAndShaderResources(ID3D11DeviceContext* *DeviceContext);
	void DefineInputAssembler(
		ID3D11DeviceContext*	*DeviceContext,
		ID3D11Buffer*			*VertexBuffer
	);


	// TO-BE PUBLIC BELOW:
	ID3D11RenderTargetView* *GetRenderTargetView();
	ID3D11Texture2D*		*GetDepthStencil();
	ID3D11DepthStencilView*	*GetDepthStencilView();


	void Initialize(ID3D11Device* *Device);


	void ReleaseAll();

};


#endif