struct PSSceneIn
{
	float4 Pos : SV_POSITION;
	float4 Normal : NORMAL;

	float3 worldPos : POSITION0;
};

float4 PS_main(PSSceneIn input) : SV_Target
{
	float3 Light_Source = { 0.0f, 0.0f, -2.01f };
	float3 Pos_To_Light;
	float DotProd;
	float3 Colour = { 1.0f, 1.0f, 1.0f };
	//Colour = Colour + input.Normal.xyz;
	//DOT PRODUCT CALCULATIONS

	Pos_To_Light = (Light_Source - input.worldPos.xyz);
	Pos_To_Light = normalize(Pos_To_Light);
	DotProd = dot(Pos_To_Light, input.Normal);
	Colour = (Colour * clamp(DotProd, 0.0f, 1.0f));

return float4(Colour, 1.0f);
};