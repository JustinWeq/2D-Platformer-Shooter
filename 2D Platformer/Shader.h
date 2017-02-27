#pragma once
#define _USE_MATH_DEFINES
#include "D3DInterface.h"
#include "Form.h"
#include "Preprocessor.h"
#include <cmath>
class Shader
{
public:
	enum SHADER_TYPE
	{
		VERTEX_SHADER,
		PIXEL_SHADER
	};

	struct Vertex
	{
		float x;
		float y;
		float texCoordX;
		float texCoordY;
		Vertex(float x,float y, float texCoordX,float texCoordY)
		{
			this->x = x;
			this->y = y;
			this->texCoordX = texCoordX;
			this->texCoordY = texCoordY;
		}
	};

	Shader();

	virtual void InitShader(D3DInterface* d3d,Form* form);

	ID3D11Buffer* GetIndexBuffer();

	ID3D11Buffer* GetVertexBuffer();


	ID3D11VertexShader* GetVertexShader();

	ID3D11PixelShader* GetPixelShader();

	virtual void Close();

	char* GetErrorMessage();
	
	ID3D11InputLayout* GetInputLayout();

	
protected:
	float GetPixelWidth();
	float GetPixelHeight();

	virtual void setUpInputLayout(D3DInterface* d3d) = 0;
	virtual void loadPixelShader(D3DInterface* d3d) = 0;
	virtual void loadVertexShader(D3DInterface* d3d) = 0;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D10Blob* m_vertexShaderBuffer;
	ID3D10Blob* m_pixelShaderBuffer;
	char* m_errorMessage;
	ID3D11InputLayout* m_layout;

private:

	ID3D11Buffer* m_vertexBuffer;
	 ID3D11Buffer* m_indexBuffer;
	 float m_pixelWidth;
	 float m_pixelHeight;
};

static void printShaderErrorMessage(ID3D10Blob* errorMessage, Form* form, WCHAR* filename)
{
	std::ofstream fout;
	char* shaderErrors = (char*)errorMessage->GetBufferPointer();
	unsigned int numOfErrors = errorMessage->GetBufferSize();
	//Open a file to write the shader error to
	fout.open("Shader_Errors.txt");
	for (int i = 0; i < numOfErrors; i++)
	{
		fout << shaderErrors[i];
	}

	//close the output message and present the error to the user
	MessageBox(form != NULL?form->GetHwnd():NULL, L"There was an error compiling the shader, check Shader_Error.txt for details", filename, MB_OK);

}

static bool CompileShader(D3DInterface* d3d,ID3D10Blob** shader,WCHAR* filename,char* shaderEntryName,Shader::SHADER_TYPE type)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	//compile the passed in shader type
	switch (type)
	{
	case Shader::VERTEX_SHADER:
	{
		//compile the vertex shader
		result = D3DX11CompileFromFile(filename, NULL, NULL, shaderEntryName, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, shader, &errorMessage, NULL);
		if (FAILED(result))
		{
			//output the shader error message
			printShaderErrorMessage(errorMessage, NULL, filename);
			return false;
		}
		break;
	}
	case Shader::PIXEL_SHADER:
	{
		//compile the vertex shader
		result = D3DX11CompileFromFile(filename, NULL, NULL, shaderEntryName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, shader, &errorMessage, NULL);
		if (FAILED(result))
		{
			printShaderErrorMessage(errorMessage, NULL, filename);
			return false;
		}
		break;
	}
	default:
		return false;
	}

	return true;
}



