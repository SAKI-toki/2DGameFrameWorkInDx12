cbuffer cbTansMatrix : register(b0)
{
	float4x4 WVP;
	float4 col;
};

Texture2D<float4> tex0 : register(t0);
SamplerState samp0 : register(s0);

struct VS_INPUT
{
	float3 Position : POSITION;
	float2 UV		: TEXCOORD;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 UV		: TEXCOORD;
};


PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;

	float4 Pos = float4(input.Position, 1.0f);
	output.Position = mul(Pos, WVP);
	output.UV = input.UV;

	return output;
}


float4 ps(PS_INPUT input) : SV_TARGET
{
	return tex0.Sample(samp0, input.UV) * col;
}
