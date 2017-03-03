#pragma once
#include "Instance.h"
#include <list>
class InstanceChannelList
{
public:
	InstanceChannelList();

	bool Initialize(bool* needsRendered, int* maxInstanceChannelSizes, int numberOfInstanceChannels,LPCWSTR* textures,D3DInterface* d3d);

	void Close();

	void DeleteInstance(Instance* data, int channel);

	void AddInstance( int channel, Instance* instance);

	ID3D11ShaderResourceView* GetTexture(int channel);

	ID3D11Buffer* GetInstanceBuffer(D3DInterface* d3d,int index);

	int GetNumberOfInstanceChannels();

	int getListCount(int list);


private:

	bool updateInstanceBuffer(D3DInterface* d3d, int channel);

	bool setUpInstanceBuffers(D3DInterface* d3d);

	bool setUpInstanceBuffer(D3DInterface* d3d, int channel, int maxSize);

	bool loadTextures(D3DInterface* d3d,LPCWSTR* textureFiles);
private:
	ID3D11ShaderResourceView** m_textures;
	bool* m_isDirty;
	bool* m_needsRendered;
	ID3D11Buffer** m_instanceBuffers;
	int m_numberOfChannels;
	int* m_channelMaxSize;
	int* m_counts;
	Instance::InstanceNode** m_head;
	Instance::InstanceNode** m_tail;
};