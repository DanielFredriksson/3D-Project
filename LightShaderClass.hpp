#ifndef LIGHTSHADERCLASS_HPP
#define LIGHTSHADERCLASS_HPP

#include <d3d11.h>
#include <d3dcompiler.h>

class LightShaderClass {
private:
	ID3D11InputLayout*		VertexLayout;
	ID3D11VertexShader*		VertexShader;
	ID3D11GeometryShader*	GeometryShader;
	ID3D11PixelShader*		PixelShader;

	ID3D11RenderTargetView*	BackBufferRTV;

public:

	LightShaderClass();
	~LightShaderClass();


	void InitializeShaders(ID3D11Device* *Device);

	void SetShadersAndShaderResources(ID3D11DeviceContext* *DeviceContext);

	void ReleaseAll();

};



#endif