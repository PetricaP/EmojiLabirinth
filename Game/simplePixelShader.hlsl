Texture2D shaderTexture;
SamplerState SampleType;

struct Input {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(Input input) : SV_TARGET {
	return shaderTexture.Sample(SampleType, input.texCoord);
}