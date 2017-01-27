#include "FrequentFunctions.hpp"



// Currently Clears:
// BackBuffer.
// 
void Clear(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11RenderTargetView* BackBufferRTV,
	ID3D11DepthStencilView*	DepthStencilView
)
{
	// clear the back buffer to a deep blue
	float clearColor[] = { 0, 0, 0, 1 };
	(*DeviceContext)->ClearRenderTargetView(BackBufferRTV, clearColor);

	// Also clear DepthBuffer via DepthStencilView
	(*DeviceContext)->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
}

LRESULT CALLBACK WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

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
	ID3D11DepthStencilView*	DepthStencilView
)
{
	static bool hasRun = false;
	Clear(DeviceContext, BackBufferRTV, DepthStencilView);

	if (!hasRun) {

		SetShadersAndShaderResources(
			DeviceContext,
			VertexShader,
			GeometryShader,
			PixelShader
		);
		DefineInputAssembler(
			DeviceContext,
			VertexBuffer,
			IndexBuffer,
			VertexLayout
		);
		hasRun = true;

	}
	else {

	}

	(*DeviceContext)->DrawIndexed(CalcRelatedData->indexCount, 0, 0);
	//(*DeviceContext)->Draw(CalcRelatedData->vertexCount, 0);

	// DrawIndexed is used when you have an index buffer along side your vertex buffer. However with this
	// system, drawing hard edges is impossible. Instead, by using the Draw functions instead, you can
	// make it so that hard edges appear. This requires that the vectorArray is allocated the following
	// memory: faceCount * 3. You need 1 array with the vertices in order, and then another array where
	// the vertices are loaded in, in the order that the faces present the vertices.
}
