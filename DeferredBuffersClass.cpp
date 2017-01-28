#include "DeferredBuffersClass.hpp"

DeferredBuffersClass::DeferredBuffersClass()
{
	for (int i = 0; i < BUFFER_COUNT; i++) {
		this->RenderTargetTextureArray[i] = nullptr;
		this->RenderTargetViewArray[i] = nullptr;
		this->ShaderResourceViewArray[i] = nullptr;
	}
	this->DepthStencilBuffer = nullptr;
	this->DepthStencilView = nullptr;
	this->ViewPort = { 0 };
}
DeferredBuffersClass::~DeferredBuffersClass()
{

}