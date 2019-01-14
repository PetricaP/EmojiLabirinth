Texture2D shaderTexture;
SamplerState SampleType;

struct Input {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float2 fake_pos : TEXCOORD1;
};

float4 main(Input input) : SV_TARGET {
	float dx = input.fake_pos.x - 0.0f;
	float dy = input.fake_pos.y - 0.0f;
	return shaderTexture.Sample(SampleType, input.texCoord) / sqrt(sqrt(dx * dx + dy * dy)) * 0.15;
}