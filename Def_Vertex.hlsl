
// Needs the matrix struct: { world,view,projection }
//
//
//
//'
cbuffer GSConstantBuffer : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;

	float4x4 X_RotationMatrix;
	float4x4 Y_RotationMatrix;
};

struct Deferred_VertexShader_Input
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
//	float2 TexUV	: TEXCOORD;
};

struct Deferred_PixelShader_Input
{
	float4 Position : SV_POSITION;
	float4 Normal : NORMAL;
	// TextureCoordinate
};


Deferred_PixelShader_Input DEF_VS_main(Deferred_VertexShader_Input input)
{
	Deferred_PixelShader_Input output;

	output.Position = float4(input.Position, 1);
	output.Normal = float4(input.Normal, 0);

	// output.texcoord = input.texcoord;

	return output;
}