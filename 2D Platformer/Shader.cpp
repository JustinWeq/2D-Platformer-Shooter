#include "Shader.h"

Shader::Shader()
{
	//init params
	m_pixelShader = NULL;
	m_vertexShader = NULL;
}

void Shader::InitShader(D3DInterface * d3d, Form * form)
{
	//store the pixel width and height
	m_pixelWidth = form->GetPixelWidth();
	m_pixelHeight = form->GetPixelHeight();

	HRESULT result;
	//init the vertex buffer if it is not null

		D3D11_BUFFER_DESC vertexBufferDescription, indexBufferDescription;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		Vertex vertices[] = { Vertex(-1.0f*GetPixelWidth(),1.0f*GetPixelHeight(),0.0f,0.0f ), Vertex(1.0f*GetPixelWidth(),1.0f*GetPixelHeight() ,1.0f,0.0f ),
			Vertex(-1.0f*GetPixelWidth(),-1.0f*GetPixelHeight(),0.0f,1.0f ),Vertex(1.0f*GetPixelWidth(),-1.0f*GetPixelHeight() ,1.0f,1.0f ) };
		unsigned long indices[] = { 0,1,2,1,3,2 };

		//Set up the description of the static vertex buffer
		vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDescription.ByteWidth = sizeof(Vertex) * 4;
		vertexBufferDescription.CPUAccessFlags = 0;
		vertexBufferDescription.MiscFlags = 0;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.StructureByteStride = 0;

		//give the subresource a pointer to the vertex data
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//create the vertex buffer now
		result = d3d->GetDevice()->CreateBuffer(&vertexBufferDescription, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			//SAFE_SET_STRING(m_errorMessage, "Failed to set the vertex buffer");
			return;
		}

		//set up the description of the index buffer
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDescription.ByteWidth = sizeof(unsigned long) * 6;
		indexBufferDescription.CPUAccessFlags = 0;
		indexBufferDescription.MiscFlags = 0;
		indexBufferDescription.StructureByteStride = 0;
		indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

		//set the subresource data for the index buffer
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		//create the index buffer
		result = d3d->GetDevice()->CreateBuffer(&indexBufferDescription, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			//SAFE_SET_STRING(m_errorMessage, "Failed to set the index buffer");
			return;
		}
	

	//load the pixel shader
	loadPixelShader(d3d);

	//load the vertex shader
	loadVertexShader(d3d);

	//set up the input layout
	setUpInputLayout(d3d);
}

ID3D11Buffer * Shader::GetIndexBuffer()
{
	return m_indexBuffer;
}

ID3D11Buffer * Shader::GetVertexBuffer()
{
	return m_vertexBuffer;
}

ID3D11VertexShader * Shader::GetVertexShader()
{
	return m_vertexShader;
}

ID3D11PixelShader * Shader::GetPixelShader()
{
	return m_pixelShader;
}

void Shader::Close()
{
	//release all the dynamically allocated data
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_vertexShader);

	delete[] m_errorMessage;
	m_errorMessage = NULL;
}

char * Shader::GetErrorMessage()
{
	return m_errorMessage;
}

ID3D11InputLayout * Shader::GetInputLayout()
{
	return m_layout;
}

float Shader::GetPixelWidth()
{
	return m_pixelWidth;
}

float Shader::GetPixelHeight()
{
	return m_pixelHeight;
}

