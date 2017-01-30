
// Needs the matrix struct: { world,view,projection }
//
//
//
//'
struct MatrixStruct
{
	// float4x4 world;
	// float4x4 view;
	// float4x4 projection;
};

struct Deferred_VertexShader_Input
{
	// Position		: POSITION
	// Normal		: NORMAL
	// TextureCoordinate
};

struct Deferred_PixelShader_Input
{
	// Position : SV_POSITION
	// Normal : 
	// TextureCoordinate
};


Deferred_PixelShader_Input DEF_VS_main(Deferred_VertexShader_Input input)
{
	Deferred_PixelShader_Input output;

	// output.pos = float4(input.pos, 1)
	// output.normal = float4(input.normal, 0);

	// output.texcoord = input.texcoord;

	return output;
}