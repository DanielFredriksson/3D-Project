#include "Initializations.hpp"


HWND InitiateWindow(HINSTANCE hInstance, LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
) {
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