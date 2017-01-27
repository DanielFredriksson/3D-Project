#include "Initializations.hpp"

HRESULT CreateDirect3DContext(
	HWND wndHandle,
	IDXGISwapChain*			*SwapChain,
	ID3D11Device*			*Device,
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11RenderTargetView*	*BackBufferRTV,
	ID3D11Texture2D*		*DepthStencil,
	ID3D11DepthStencilView* *DepthStencilView
)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		SwapChain,
		Device,
		NULL,
		DeviceContext);

	// ~ DepthBuffer
	InitialiseDepthStencilAndView(Device, DepthStencil, DepthStencilView);  // Has to be called after we've created Device,DeviceContext and SwapChain // Has to be called before we set render targets
																			//
	if (SUCCEEDED(hr))
	{

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		(*SwapChain)->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		(*Device)->CreateRenderTargetView(pBackBuffer, NULL, BackBufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		(*DeviceContext)->OMSetRenderTargets(1, BackBufferRTV, *DepthStencilView);

	}
	return hr;
}

void CreateViewport(ID3D11DeviceContext* *DeviceContext
){
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	(*DeviceContext)->RSSetViewports(1, &vp);
}

HWND InitiateWindow(HINSTANCE hInstance, LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
){
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

void DefineInputAssembler(
	ID3D11DeviceContext*	*DeviceContext,
	ID3D11Buffer*			*VertexBuffer,
	ID3D11Buffer*			*IndexBuffer,
	ID3D11InputLayout*		VertexLayout
)
{
	UINT32 vertexSize = sizeof(DirectX::XMFLOAT3) * 2;	//Data amount per INDIVIDUAL VERTEX
	UINT32 offset = 0;

	(*DeviceContext)->IASetVertexBuffers(0, 1, VertexBuffer, &vertexSize, &offset);
	(*DeviceContext)->IASetIndexBuffer(*IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	(*DeviceContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	(*DeviceContext)->IASetInputLayout(VertexLayout);
}

void CreateObjectData(
	ID3D11Device* *Device,
	ID3D11Buffer* *VertexBuffer,
	ID3D11Buffer* *IndexBuffer,
	objData* *object,
	calcData *data
)
{
	HRESULT hr;

	// Initialize VertexBuffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = (sizeof(objData) * data->vertexCount); //Alt: (sizeof(DirectX::XMFLOAT3) * data.vertexCount)

	D3D11_SUBRESOURCE_DATA vertexData;
	memset(&vertexData, 0, sizeof(vertexData));
	vertexData.pSysMem = (*object);	//POSSIBLE ERROR
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	hr = (*Device)->CreateBuffer(&vertexBufferDesc, &vertexData, VertexBuffer);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Vertex Buffer'.", L"ERROR: 'CreateObjectData() in Initializations.cpp.'", MB_OK);

	// Initialize IndexBuffer
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = (sizeof(unsigned long) * data->indexCount);	//POSSIBLE ERROR

	D3D11_SUBRESOURCE_DATA indexData;
	memset(&indexData, 0, sizeof(indexData));
	indexData.pSysMem = data->indices;	//POSSIBLE ERROR
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = (*Device)->CreateBuffer(&indexBufferDesc, &indexData, IndexBuffer);

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create 'Vertex Buffer'.", L"ERROR: 'CreateObjectData() in Initializations.cpp.'", MB_OK);
}