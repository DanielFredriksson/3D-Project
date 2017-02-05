#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <codecvt>
#include <locale>

#include "Globals.hpp"
#include "Object.hpp"





//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									NECESSARY STRUCTS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+






//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//																					//
//									OBJECTLOADER CLASS								//
//																					//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

class objLoaderClass
{
private:
	objDataClass* bufferData;
	calcDataClass CalcRelatedData;


	DirectX::XMFLOAT3 *temp_vertices;
	DirectX::XMFLOAT2 *temp_texCoords;
	DirectX::XMFLOAT3 *temp_normals;

	std::ifstream input_file;

public:
	objLoaderClass();
	~objLoaderClass();

	/* ------------- COMMENTS -------------
	ImportObjectFile imports all data from the Obj/Mtl-files which correspond
	to 'fileName' and inserts it into the 'Object' parameter.
	*/
	void importObjFile(ObjectClass *Object);

	objDataClass* *get_objData();
	calcDataClass *get_calcData();
};

#endif