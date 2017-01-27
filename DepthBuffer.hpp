#ifndef DEPTHBUFFER_HPP
#define DEPTHBUFFER_HPP

#include <d3d11.h>
#include "Globals.hpp"

//	 ACTIONS REQUIRED:
// - Initialise DepthStencil and DepthStencilView	| ~CreateDirect3DContext()
// - Set DepthStencilState as a render target		| ~CreateDirect3DContext()
// - Clear DepthStencilView each render loop		| ~Render() ~ ~Clear()

void InitialiseDepthStencilAndView(
	ID3D11Device*			*Device,
	ID3D11Texture2D*		*DepthStencil,
	ID3D11DepthStencilView* *DepthStencilView
);

// The DepthStencilBuffer (Data Type: ID3D11Texture2D) is where all the depth related data is
// stored for each individual pixel. This is what is accessed through the to-be-mentioned
// 'pointer'. NOTE: A ID3D11Texture2D is used to store a variety of different data, such as the
// information provided by the BTHLOGO.txt used in Assignment 2 (Course: DV1542), for example.

// The DepthStencilView(Data Type : ID3D11DepthStencilView) can be seen as a type of pointer.
// This pointer is what we use to be able to access and alter the depth buffer and its data.

// The DepthStencilState (Data Type: ID3D11DepthStencilState) contains "state" that controls
// how depth and stencil data impacts rendering. NOTE: 'depthStencil' data is stored as
// "floating-point data or z data"-, whereas "stencilBuffer stores integer data", for each pixel.


#endif