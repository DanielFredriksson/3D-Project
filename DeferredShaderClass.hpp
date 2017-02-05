#ifndef DEFERREDSHADERCLASS_HPP
#define DEFERREDSHADERCLASS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

#include "ConstantBufferClass.hpp"	// Needs MatrixBufferStored

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

	ID3D11VertexShader* VertexShader;
	ID3D11InputLayout*	VertexLayout;
	ID3D11PixelShader*	PixelShader;
	ID3D11SamplerState*	SamplerStateWrap; // Wraps Address.X



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
	Updates the internal MatrixBuffer with values from the matrices given as input and
	sets the internal matrixbuffer to the VertexShader.
	After that it sets the internal VertexLayout, VertexShader, PixelShader & SamplerState to
	the current pipeline.
	Finally calls Draw();
	*/



	/* ------------- COMMENTS -------------
	Sets these to the pipeline:
	VertexLayout.
	VertexShader.
	PixelShader.
	SamplerState.
	*/
	void SetShadingContext(ID3D11DeviceContext* *DeviceContext);



	/* ------------- COMMENTS -------------
	Replaces the current VertexLayout with the internal VertexLayout.
	Replaces both Pixel/VertexShader.
	Sets the internal SamplerState to the current(the internal) PixelShader.
	Then calls DeviceContext->Draw(VerticeCount, 0)
	*/
	void RenderShader(ID3D11DeviceContext* *DeviceContext, int VerticeCount);





	/* ------------- COMMENTS -------------
	*/
	void ReleaseAll();
};




#endif