#ifndef DEFERREDSHADERCLASS_HPP
#define DEFERREDSHADERCLASS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

#include "ConstantBuffers.hpp" // Class needs access to GSConstantDataMatrices

// CURRENTLY NO TEXTURES - TO CHANGE, EDIT:
//	InitialiseShaders()		- "TEXCOORD"
//	Def_Vertex.hlsl			- Input - "TEXCOORD"
//	Def_Pixel.hlsl			- Input - "TEXCOORD"
//	SetShaderParameters()
//	Render()
//
//	SamplerState is still created and set as if textures were implemented.
//
//



class DeferredShaderClass {
private:
	struct MatrixBuffer
	{
		// world
		// view
		// projection
	};

	ID3D11PixelShader* PixelShader;
	ID3D11VertexShader* VertexShader;
	ID3D11InputLayout*	VertexLayout;

	ID3D11SamplerState*	SamplerStateWrap; // Wraps Address.X
	ID3D11Buffer*		MatrixBuffer;


	/* ------------- COMMENTS -------------
	This function maps updates the internal MatrixBuffer with world, view, projection
	through Map/Unmap.
	Then sets the internal MatrixBuffer as a ConstantBuffer to the current VertexShader.
	Also sets the recieved ShaderResourceView-texture to the current PixelShader.
	*/
	void SetShaderParameters(
		ID3D11DeviceContext*		*DeviceContext,
	//	ID3D11ShaderResourceView*	*texture,
		DirectX::XMFLOAT4X4			FormattedWorldMatrix,
		DirectX::XMFLOAT4X4			FormattedViewMatrix,
		DirectX::XMFLOAT4X4			FormattedProjectionMatrix
	);

	/* ------------- COMMENTS -------------
	Replaces the current VertexLayout with the internal VertexLayout.
	Replaces both Pixel/VertexShader.
	Sets the internal SamplerState to the current(the internal) PixelShader.
	Then calls DeviceContext->Draw(VerticeCount, 0)
	*/
	void RenderShader(ID3D11DeviceContext* *DeviceContext, int VerticeCount);

public:
	DeferredShaderClass();
	~DeferredShaderClass();

	/* ------------- COMMENTS -------------
	Creates internal Vertex/Pixel shaders.
	Creates internal SamplerState.
	Creates internal MatrixBuffer.
	*/
	void InitialiseShaders(ID3D11Device* *Device);

	/* ------------- COMMENTS -------------
	Simply sets the shader parameters then renders the shader.
	SetShaderParameters() + RenderShader()	
	*/
	void Render(
		ID3D11DeviceContext*		*DeviceContext,
	//	ID3D11ShaderResourceView*	*texture,
		DirectX::XMFLOAT4X4			FormattedWorldMatrix,
		DirectX::XMFLOAT4X4			FormattedViewMatrix,
		DirectX::XMFLOAT4X4			FormattedProjectionMatrix,
		int							VerticeCount
	);

	/* ------------- COMMENTS -------------
	
	*/
	void ReleaseAll();
};




#endif