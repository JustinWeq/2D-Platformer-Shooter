cbuffer ConstantBuffer
{
	float camX;
	float camY;
};

struct VertexInput
{
	float2 position:POSITION;
	float2 texCoords: TEXCOORD;
	float2 instancePosition:POSITION1;
	float2 size:POSITION2;
	float2 uVAdd:TEXCOORD1;
	float2 uVMultiply: TEXCOORD2;
};

struct PixelInput
{
	float4 position:SV_POSITION;
	float2 texCoords:TEXCOORD0;
};

Texture2D tex;
SamplerState samplerState;

PixelInput vertexShader(VertexInput input)
{
	PixelInput output;
	float2 uv;
	uv.x = input.texCoords.x == 0 ? input.uVAdd.x : input.uVMultiply.x;
	uv.y = input.texCoords.y == 0 ? input.uVAdd.y : input.uVMultiply.y;
	output.texCoords =uv;
	output.position = float4((input.position.x*input.size.x) + input.instancePosition.x - camX,(input.size.y*input.position.y) + input.instancePosition.y- camY, 0.0f, 1.0f);

	return output;
}

float4 pixelShader(PixelInput input) : SV_TARGET
{
	float4 color = tex.Sample(samplerState,input.texCoords);
	if (color.r*color.g*color.b ==0)
		color.a = 0.0f;
	return color;
}