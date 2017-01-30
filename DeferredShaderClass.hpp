#ifndef DEFERREDSHADERCLASS_HPP
#define DEFERREDSHADERCLASS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

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

	ID3D11SamplerState*	SamplerStateWrap;	// Wrap?
	ID3D11Buffer*		MatrixBuffer;


	// OutputShaderErrorMessage()
	// SetShaderParameters()
	

public:

	DeferredShaderClass();
	~DeferredShaderClass();

	void InitialiseShaders(ID3D11Device* *Device);
	void ReleaseAll();
	// Render




};




#endif