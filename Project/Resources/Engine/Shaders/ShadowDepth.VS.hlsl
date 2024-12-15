
struct LightMatrix {
	
	float4x4 lightViewProjection;
};

struct VertexShaderInput {
	
	float3 position : POSITION;
	
	// •K—v‚È‚¢‚¯‚Ç‚ß‚ñ‚Ç‚¢‚Ì‚Å“ü‚ê‚é
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexShaderOutput {
	
	float4 position : SV_POSITION;
};

ConstantBuffer<LightMatrix> gLightMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input) {
	
	VertexShaderOutput output;
	output.position = mul(float4(input.position, 1.0f), gLightMatrix.lightViewProjection);
	
	return output;
}