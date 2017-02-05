#ifndef DEFERREDBUFFERSCLASS_HPP
#define DEFERREDBUFFERSCLASS_HPP

// The DeferredBufferClass is just the "Render-To-Texture" Class re-written to handle an array
// of "Render-To-Texture"'s instead of just a single one. The outputs of the Deferred Shader
// will be writing to the texture buffers (render targets) of thiss class.

#include <d3d11.h>
#include <DirectXMath.h>

#define BUFFER_COUNT 2
//	DepthBuffer
//	NormalBuffer

/*
 - Is the shaderResourceView what i think it is?

 - What purpose does the internal ViewPort serve?

 - We set the render targets to the Outputmerger,
	and the viewport to the Rasterizer, but where do we specify that these
	are in the Deferred-Pipeline and not the normal pipeline?

 - After we've set the render targets to the OutputMerger, how do we code the
	Def_Pixel_hlsl to output normals to one Texture2D and Color to another Texture2D?
	- SV_Target0 gets colour, SV_Target1 gets normal.
	

*/




class DeferredBuffersClass {
private:

	// Used as rendertargets to which data will be outputted from Deferred Shader
	ID3D11Texture2D* RenderTargetTextureArray[BUFFER_COUNT];
	// Used as a lens to work with the RenderTargetTextureArray above.
	ID3D11RenderTargetView* RenderTargetViewArray[BUFFER_COUNT];

	// Used as a lens to work with textures(?)
	ID3D11ShaderResourceView* ShaderResourceViewArray[BUFFER_COUNT];

	ID3D11Texture2D* DepthStencilBuffer;
	ID3D11DepthStencilView* DepthStencilView;




public:

	DeferredBuffersClass();
	~DeferredBuffersClass();

	/* ------------- COMMENTS -------------
	Creates RenderTargetTexture's & RenderTargetView's & ShaderResourceView's.
	Creates DepthBuffer & DepthStencilView & Viewport.
	*/
	void InitializeBuffers(
		ID3D11Device* *Device,
		UINT ScreenWidth,
		UINT ScreenHeight
	);

	/* ------------- COMMENTS -------------
	Sets all the internal RenderTargetView's to the OutputMerger stage in the GPU-pipeline.
	The order of the RenderTargets in the array is the order of "SV_TargetX".
	*/
	void SetAllRenderTargets(ID3D11DeviceContext* *DeviceContext);

	void ClearAllRenderTargets(
		ID3D11DeviceContext*	*DeviceContext,
		float					ClearColor_Red,
		float					ClearColor_Blue,
		float					ClearColor_Green,
		float					ClearColor_Alpha
	);

	void ReleaseAll();





	// Get Shader Resource View


};












#endif