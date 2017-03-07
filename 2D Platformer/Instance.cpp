#include "Instance.h"


Instance::Instance(BaseShader::BaseShaderInstance instanceData, unsigned char instanceChannel)
{
	//store the instance data
	m_instanceData = &instanceData;

	//sotre the instance channel
	m_instanceChannel = instanceChannel;

	//set up the node
	m_node = new InstanceNode(NULL, NULL, this);
}

void Instance::SetLeftRightNodes(InstanceNode * left, InstanceNode * right)
{
	//set the left and right nodes
	this->m_node->left = left;
	this->m_node->right = right;
}

Instance::InstanceNode* Instance::GetNode()
{
	return m_node;
}


void Instance::SetInstanceData(float x, float y, float width, float height)
{
	//set the data in the instance
	m_instanceData->x = x;
	m_instanceData->y = y;
	m_instanceData->height = height;
	m_instanceData->width = width;
}

void Instance::SetInstanceData(BaseShader::BaseShaderInstance instanceData)
{
	// set the instance data by copying it into the pointer
	m_instanceData->x = instanceData.x;
	m_instanceData->height = instanceData.height;
	m_instanceData->width = instanceData.width;
	m_instanceData->y = instanceData.y;
	m_instanceData->uAdd = instanceData.uAdd;
	m_instanceData->vAdd = instanceData.vAdd;
	m_instanceData->uMultiply = instanceData.uMultiply;
	m_instanceData->vMultiply = instanceData.vMultiply;
}


BaseShader::BaseShaderInstance *Instance::GetInstanceData()
{
	return m_instanceData;
}

unsigned char Instance::GetInstanceChannel()
{
	return m_instanceChannel;
}

Instance::InstanceNode::InstanceNode(InstanceNode* left, InstanceNode* right, Instance* data)
{
	this->right = right;
	this->left = left;
	this->data = data;
}
