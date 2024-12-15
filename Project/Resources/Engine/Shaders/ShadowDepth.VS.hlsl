
struct LightMatrix {
	
	float4x4 lightViewProjection;
};

struct VertexShaderInput {
	
	float3 position : POSITION;
	
	// 必要ないけどめんどいので入れる
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