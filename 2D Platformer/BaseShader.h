#pragma once
#include "Shader.h"

class BaseShader : public Shader
{
public:
	struct ConstantBuffer {
		float camX;
		float camY;

		ConstantBuffer(float camX, float camY)
		{
			this->camX = camX;
			this->camY = camY;

		}
	};


	struct BaseShaderInstance {
		float x;
		float y;
		float width;
		float height;
		BaseShaderInstance(float x, float y,float width,float height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
	};
	BaseShader();

	void updateConstantBuffer(D3DInterface* d3d,float camX, float camY);

	void Render(D3DInterface* d3d);

protected:

	virtual void setUpInputLayout(D3DInterface* d3d) override;
	virtual void loadPixelShader(D3DInterface* d3d) override;
	virtual void loadVertexShader(D3DInterface* d3d) override;

	ID3D11Buffer* m_instanceBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11ShaderResourceView* m_texture;
	ID3D11SamplerState* m_sampler;
};