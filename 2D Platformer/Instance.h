#pragma once
#include "BaseShader.h"
#include <list>
class Instance {
public:

	struct InstanceNode
	{
		InstanceNode* left;
		InstanceNode* right;
		Instance* data;

		InstanceNode(InstanceNode* left, InstanceNode* right, Instance* data);
	};

	Instance(BaseShader::BaseShaderInstance instanceData, unsigned char instanceChannel);

	void SetLeftRightNodes(InstanceNode* left = NULL, InstanceNode* right = NULL);

	InstanceNode* GetNode();


	void SetInstanceData(float x, float y,float width,float height);

	void SetInstanceData(BaseShader::BaseShaderInstance instanceData);


	BaseShader::BaseShaderInstance* GetInstanceData();

	unsigned char GetInstanceChannel();

private:

	BaseShader::BaseShaderInstance* m_instanceData;
	unsigned char m_instanceChannel;
	InstanceNode* m_node;
};