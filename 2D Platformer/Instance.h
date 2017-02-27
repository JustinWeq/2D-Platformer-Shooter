#pragma once
#include "BaseShader.h"
class Instance {
public:

	Instance(BaseShader::BaseShaderInstance instanceData, unsigned char instanceChannel, int instanceID);

	void SetInstanceID(int instanceID);

	void SetInstanceData(float x, float y,float width,float height);

	void SetInstanceData(BaseShader::BaseShaderInstance instanceData);

	int GetInstanceID();

	BaseShader GetInstanceData();

	unsigned char GetInstanceChannel();

private:

	BaseShader::BaseShaderInstance m_instanceData;
	unsigned char m_instanceChannel;
	int m_instanceID;
};