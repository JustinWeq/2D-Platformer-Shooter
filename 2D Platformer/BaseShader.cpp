#include "BaseShader.h"

BaseShader::BaseShader():Shader::Shader()
{
}

void BaseShader::updateConstantBuffer(D3DInterface* d3d,float camX, float camY)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int resourceID;
	ConstantBuffer* buffer;
	//update the screen width and height if they need updated
	


	//lock the constant buffer so it can be written to
	result = d3d->GetImmediateContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	//get a reference to the data in the mapped resource
	buffer =(ConstantBuffer*) mappedResource.pData;
	//copy the info into the update constant buffer function
	buffer->camX = camX;
	buffer->camY = camY;

	//unlock the constant buffer
	d3d->GetImmediateContext()->Unmap(m_constantBuffer, 0);

	//set the constant buffer
	d3d->GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer);

}

void BaseShader::Render(D3DInterface * d3d)
{
	ID3D11Buffer* vertexBuffers[2];
	ID3D11Buffer* indexBuffer = GetIndexBuffer();
	unsigned int strides[2];
	unsigned int offsets[2];
	//set the data to be drawn
	vertexBuffers[0] = GetVertexBuffer();
	vertexBuffers[1] = m_instanceBuffer;

	strides[0] = sizeof(Vertex);
	strides[1] = sizeof(BaseShaderInstance);

	offsets[0] = 0;
	offsets[1] = 0;

	ID3D11DeviceContext* deviceContext= d3d->GetImmediateContext();

	//turn on alpha blending
	d3d->EnableAlphaBlending();

	//set primitive topology
	d3d->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	d3d->GetImmediateContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	deviceContext->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
	//set the input layout
	d3d->GetImmediateContext()->IASetInputLayout(m_layout);

	//set the vertex and pixel shader
	d3d->GetImmediateContext()->VSSetShader(m_vertexShader, NULL, 0);
	d3d->GetImmediateContext()->PSSetShader(m_pixelShader, NULL, 0);

	//set the shader resources
	deviceContext->PSSetShaderResources(0, 1, &m_texture);
	deviceContext->PSSetSamplers(0, 1,&m_sampler);

	//draw the model quad
	d3d->GetImmediateContext()->DrawIndexedInstanced(6, 2, 0,0,0);
}

void BaseShader::RenderInstance(D3DInterface * d3d, ID3D11Buffer * instanceBuffer, ID3D11ShaderResourceView * texture, int numberOfInstances)
{
	ID3D11Buffer* vertexBuffers[2];
	ID3D11Buffer* indexBuffer = GetIndexBuffer();
	unsigned int strides[2];
	unsigned int offsets[2];
	//set the data to be drawn
	vertexBuffers[0] = GetVertexBuffer();
	vertexBuffers[1] = instanceBuffer;

	strides[0] = sizeof(Vertex);
	strides[1] = sizeof(BaseShaderInstance);

	offsets[0] = 0;
	offsets[1] = 0;

	ID3D11DeviceContext* deviceContext = d3d->GetImmediateContext();

	//turn on alpha blending
	d3d->EnableAlphaBlending();

	//set primitive topology
	d3d->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	d3d->GetImmediateContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	deviceContext->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
	//set the input layout
	d3d->GetImmediateContext()->IASetInputLayout(m_layout);

	//set the vertex and pixel shader
	d3d->GetImmediateContext()->VSSetShader(m_vertexShader, NULL, 0);
	d3d->GetImmediateContext()->PSSetShader(m_pixelShader, NULL, 0);

	//set the shader resources
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &m_sampler);

	//draw the model quad
	d3d->GetImmediateContext()->DrawIndexedInstanced(6, numberOfInstances, 0, 0, 0);
}

void BaseShader::setUpInputLayout(D3DInterface * d3d)
{
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	//set up the instance buffer
	BaseShaderInstance instance[] = { BaseShaderInstance(-100.0f*GetPixelWidth(),0.0f,100,100),BaseShaderInstance(0.0f,0.0f,100,100) };

	//set up the parameters for the instance buffer desc
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.ByteWidth = sizeof(BaseShaderInstance) * 2;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//set up the data for the instance buffer
	ZeroMemory(&instanceData, sizeof(D3D11_SUBRESOURCE_DATA));
	instanceData.pSysMem = instance;

	//actually create the buffer now
	d3d->GetDevice()->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);


	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	HRESULT result;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].SemanticName = "POSITION";

	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].SemanticName = "TEXCOORD";

	//instance step rate
	polygonLayout[2].AlignedByteOffset = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].InstanceDataStepRate = 1;
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].SemanticName = "POSITION";
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;

	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[3].InputSlot = 1;
	polygonLayout[3].InstanceDataStepRate = 1;
	polygonLayout[3].SemanticIndex = 2;
	polygonLayout[3].SemanticName = "POSITION";
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;


	//create the polygon layut
	result = d3d->GetDevice()->CreateInputLayout(polygonLayout,4, m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		////SAFE_SET_STRING(m_errorMessage, "There was an error creating the vertex input layout");
		int test = 3;
	}

	//create the constant buffer
	D3D11_BUFFER_DESC constBuffDesc;

	constBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBuffDesc.ByteWidth = sizeof(ConstantBuffer)+8;
	constBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBuffDesc.MiscFlags = 0;
	constBuffDesc.StructureByteStride = 0;

	m_constantBuffer = NULL;
	//create the buffer
	result = d3d->GetDevice()->CreateBuffer(&constBuffDesc, NULL, &m_constantBuffer);
	if (FAILED(result))
	{
		int test = 3;
		//do nothing
		return;
	}

	//load the texture
	result = D3DX11CreateShaderResourceViewFromFile(d3d->GetDevice(), L"Smile.dds", NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		int test = 3;
		return;
	}


	//create the sampler state description
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 1.0f;
	samplerDesc.MipLODBias = 0.0f;

	//now actually create the texture sampler state
	result = d3d->GetDevice()->CreateSamplerState(&samplerDesc, &m_sampler);
	if (FAILED(result))
	{
		int test = 3;
		return;
	}


}

void BaseShader::loadPixelShader(D3DInterface * d3d)
{

	HRESULT result;
	//attempt to load the pixel shader
	if (!CompileShader(d3d, &m_pixelShaderBuffer, TEXT("BaseVertexShader.hlsl"), "pixelShader", PIXEL_SHADER))
	{
		return;
		//shader failed to compile
		////SAFE_SET_STRING(m_errorMessage, "There was an error compiling the pixel shader")
		//there was an error comping
	}
	else
	{
		//create the vertex shader
		result = d3d->GetDevice()->CreatePixelShader(m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
		if (FAILED(result))
		{
			int test = 3;
			////SAFE_SET_STRING(m_errorMessage, "There was an error creating the pixel shader");
		}
	}

}

void BaseShader::loadVertexShader(D3DInterface * d3d)
{
	HRESULT result;

	//attempt to load the vertex shader
	if (!CompileShader(d3d, &m_vertexShaderBuffer, TEXT("BaseVertexShader.hlsl"), "vertexShader", VERTEX_SHADER))
	{
		//shader failed to compile
		////SAFE_SET_STRING(m_errorMessage,"There was an error creating the vertex shader")
	}
	else
	{
		//create the vertex shader
		result = d3d->GetDevice()->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
		if (FAILED(result))
		{
			return;
			////SAFE_SET_STRING(m_errorMessage, "There was an error creating the vertexshader");
		}
	}
}



