struct VertexInput
{
	float2 position:POSITION;
};

struct PixelInput
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

PixelInput VertexShader(VertexInput input)
{
	PixelInput output;
	output.x = input.x;
	output.y = input.y;
	output.z = 0.0f;
	output.w = 1.0f;

	output.color = float4(0.0f, 1.0f, 0.0f, 1.0f);

	return output;
}

float4 PixelShader(PixelInput input)
{
	return input.color;
}