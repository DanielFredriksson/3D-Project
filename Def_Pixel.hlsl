
// The texture(?)
// --- Texture2D shaderTexture : register(t0)


// SamplerState which uses the interpolated TextureCoordinates to sample the correct
// colour from the texture.
// --- SamplerState SampleTypeWrap : register(s0)


struct Deferred_PixelShader_Input
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	// TextureCoordinate
};

// Data which will be rendered to the 'RenderTargetView's inside the 'DeferredBuffersClass'.
struct RenderTarget_Input
{
	float4 colour : SV_Target0;
	float3 normal : SV_Target1;
};


RenderTarget_Input DEF_PS_main(Deferred_PixelShader_Input input) : SV_Target
{
	RenderTarget_Input output;
	
	output.colour = input.Position;
	output.normal = input.Normal;
	// Sample texture coordinates
	// --- output.colour = shaderTexture.Sample(SampleTypeWrap, input.textureCoordinate);

	// --- output.normal = float4(input.normal, 0.0f);

	return output;

	

}; // ';' necessary?