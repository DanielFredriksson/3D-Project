
// The texture(?)
// --- Texture2D shaderTexture : register(t0)


// SamplerState which uses the interpolated TextureCoordinates to sample the correct
// colour from the texture.
// --- SamplerState SampleTypeWrap : register(s0)

cbuffer GSConstantBuffer : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;

	float4x4 X_RotationMatrix;
	float4x4 Y_RotationMatrix;
};

struct Deferred_PixelShader_Input
{
	float4 Position : SV_POSITION;
	float4 Normal : NORMAL;
	// TextureCoordinate
};

// Data which will be rendered to the 'RenderTargetView's inside the 'DeferredBuffersClass'.
struct RenderTarget_Input
{
	float4 ColorTexture2D : SV_Target0;
	float4 NormalTexture2D : SV_Target1;
};


RenderTarget_Input DEF_PS_main(Deferred_PixelShader_Input input) : SV_Target
{
	RenderTarget_Input output;
	
	output.ColorTexture2D = input.Position;
	output.NormalTexture2D = input.Normal;



	return output;

	

} // ';' necessary?






  // Sample texture coordinates
  // --- output.colour = shaderTexture.Sample(SampleTypeWrap, input.textureCoordinate);

  // --- output.normal = float4(input.normal, 0.0f);