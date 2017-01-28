#include "objLoader.hpp"

objLoader::objLoader()
{
	this->normal_count = 0;
	this->face_count = 0;

	this->faces = nullptr;
	this->vectorVertices = nullptr;
	this->vectorNormals = nullptr;
	this->vector1 = { 0 };
	this->vector2 = { 0 };
	this->vector3 = { 0 };
}

objLoader::~objLoader()
{

}

void objLoader::importObjFile(std::string fileName)
{
	input_file.open(fileName);
	if (!input_file.is_open())
	{
		MessageBox(NULL, L"Failed to open file 'teddy.obj'.", L"ERROR: 'importObjFile()'", MB_OK);
	}

	else
	{
		float currentValue;				// Holds the current float
		std::string currentString;		// Where text/numbers are initially stored when read from file
		float float1, float2, float3;	// Holds x,y,z float values
		int int1, int2, int3;			// Holds the v1,v2,v3 vertex values for a face





		input_file >> currentString;	// Reads in next line (should be "vc")

										// -- ACQUIRING VERTEX COUNT AS WELL AS ALLOCATING MOST MEMORY --
		if (currentString == "vc")
		{
			input_file >> this->CalcRelatedData.vertexCount;

			(this->bufferData) = new objData[this->CalcRelatedData.vertexCount];	// Allocating memory (1 vertex + 1 normal)

			vectorNormals = new DirectX::XMVECTOR[this->CalcRelatedData.vertexCount];	// These are for CALCULATIONS
			vectorVertices = new DirectX::XMVECTOR[this->CalcRelatedData.vertexCount];	// and are discarded at the end.
		}





		input_file >> currentString;	// Reads in next line (should be "fc")

										// -- ACQUIRING FACE COUNT AS WELL AS ALLOCATING ITS MEMORY --
		if (currentString == "fc")
		{
			input_file >> face_count;

			faces = new DirectX::XMFLOAT3[face_count];	// Used for CALCULATIONS; doesn't get saved
			this->CalcRelatedData.indexCount = (face_count * 3);
			this->CalcRelatedData.indices = new unsigned long[this->CalcRelatedData.indexCount];
		}





		// -- STORING EACH VERTEX IN 2 SEPARATE DATA TYPES --
		for (int i = 0; i < this->CalcRelatedData.vertexCount; i++)
		{

			input_file >> currentString;		// -- READS and DISCARDS each 'v' --

			input_file >> currentString;		// X
			float1 = std::stod(currentString);

			input_file >> currentString;		// Y
			float2 = std::stod(currentString);

			input_file >> currentString;		// Z
			float3 = std::stod(currentString);

			float3 *= -1.0f;	// Converting from right-handed to left-handed system

			this->bufferData[i].vertex = { float1, float2, float3 };	// Stored in FLOAT3 form
			vectorVertices[i] = { float1, float2, float3 };				// Stored in VECTOR form
		}





		// -- LOOP FOR CALCULATING & ADDING NORMALS + STORING INDICES --
		for (int j = 0; j < face_count; j++)
		{
			input_file >> currentString;		// -- READS and DISCARDS each 'f' --

			input_file >> currentString;		// Vertec1
			int1 = std::stod(currentString);

			input_file >> currentString;		// Vertex2
			int2 = std::stod(currentString);

			input_file >> currentString;		// Vertex3
			int3 = std::stod(currentString);

			int1 -= 1;	//
			int2 -= 1;	// Because an index start at 0, not 1
			int3 -= 1;	//

			this->CalcRelatedData.indices[(j * 3) + 0] = int3;		// Here we store the values from each face backwards,
			this->CalcRelatedData.indices[(j * 3) + 1] = int2;		// as in the 3rd vertex -> 1st vertex, etc. This is
			this->CalcRelatedData.indices[(j * 3) + 2] = int1;		// done to convert from right-hand to left-hand sys.

			vector1 = DirectX::XMVectorSubtract(	// Triangle Face's Side (#1)
				vectorVertices[int1],
				vectorVertices[int2]
			);

			vector2 = DirectX::XMVectorSubtract(	// Triangle Face's Side (#2)
				vectorVertices[int1],
				vectorVertices[int3]
			);

			vector3 = DirectX::XMVector3Cross(vector2, vector1);	// The Triangle Face's normal

			vectorNormals[int1] = DirectX::XMVectorAdd(vectorNormals[int1], vector3); // Adding the normal
			vectorNormals[int2] = DirectX::XMVectorAdd(vectorNormals[int2], vector3); // to all three
			vectorNormals[int3] = DirectX::XMVectorAdd(vectorNormals[int3], vector3); // vertices.
		}





		// --	LOOP FOR NORMALIZING & STORING ALL NORMALS --
		for (int k = 0; k < this->CalcRelatedData.vertexCount; k++)
		{
			vectorNormals[k] = DirectX::XMVector3Normalize(vectorNormals[k]); // Normalize all normals

			DirectX::XMStoreFloat3(&this->bufferData[k].normal, vectorNormals[k]); // Storing all normals
			this->bufferData[k].normal.z = this->bufferData[k].normal.z * -1.0f; // Converting normals from a right-hand
		}														   // system to a left-hand system.
	}





	if (!input_file.is_open())
		std::string currentString = "Opps!";	// This is for DEBUGGING purposes; leave this breakpoint

	if (input_file.is_open())
		input_file.close();
}

objData* *objLoader::get_objData()
{
	return &this->bufferData;
}

calcData *objLoader::get_calcData()
{
	return &this->CalcRelatedData;
}