#pragma once
#include "Instance.h"

class InstanceChannelList
{

private:
	int* m_counts;
	BaseShader** instanceData;
	Instance** m_instances;
	ID3D11ShaderResourceView** m_textures;
	bool* m_isDirty;
	bool* m_needsRendered;
	ID3D11Buffer** m_instanceBuffers;
	int** m_channelMaxSize;
};