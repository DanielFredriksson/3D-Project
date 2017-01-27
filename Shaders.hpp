#ifndef SHADERS_HPP
#define SHADERS

#include <d3d11.h>
#include <d3dcompiler.h>

void CreateShadersFunc(
	ID3D11Device			*Device,
	ID3D11VertexShader*		*VertexShader,
	ID3D11GeometryShader*	*GeometryShader,
	ID3D11PixelShader*		*PixelShader,
	ID3D11InputLayout*		*VertexLayout
);

void SetShadersAndShaderResources(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11VertexShader*		VertexShader,
	ID3D11GeometryShader*	GeometryShader,
	ID3D11PixelShader*		PixelShader
);


#endif