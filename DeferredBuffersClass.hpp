#ifndef DEFERREDBUFFERSCLASS_HPP
#define DEFERREDBUFFERSCLASS_HPP

// The DeferredBufferClass is just the "Render-To-Texture" Class re-written to handle an array
// of "Render-To-Texture"'s instead of just a single one. The outputs of the Deferred Shader
// will be writing to the texture buffers (render targets) of thiss class.

#include <d3d11.h>

#define BUFFER_COUNT 2
//	DepthBuffer
//	NormalBuffer



class DeferredBuffersClass {
private:

	// Used as rendertargets to which data will be outputted from Deferred Shader
	ID3D11Texture2D* RenderTargetTextureArray[BUFFER_COUNT];
	// Used as a lens to work with the RenderTargetTextureArray above.
	ID3D11RenderTargetView* RenderTargetViewArray[BUFFER_COUNT];

	// Not sure atm.
	ID3D11ShaderResourceView* ShaderResourceViewArray[BUFFER_COUNT];

	// Will be one of the indexes in RenderTargetTextureArray
	ID3D11Texture2D* DepthStencilBuffer;
	// Used as a lens to work with the DepthStencilBuffer above.
	ID3D11DepthStencilView* DepthStencilView;

	// Not sure atm.
	D3D11_VIEWPORT ViewPort;


public:

	DeferredBuffersClass();
	~DeferredBuffersClass();

	// Initialise
	// ShutDown / Release 

	// Set Render Targets
	// Clear Render Targets

	// Get Shader Resource View


};












#endif