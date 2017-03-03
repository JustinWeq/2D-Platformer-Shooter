#include "InstanceChannelList.h"

bool InstanceChannelList::loadTextures(D3DInterface* d3d,LPCWSTR * textureFiles)
{
	//nintialize the textures
	m_textures = new ID3D11ShaderResourceView*[m_numberOfChannels];
	HRESULT result;
	//load in all of the textures
	for (int i = 0;i < m_numberOfChannels; i++)
	{
		//load he texture at i
		result = D3DX11CreateShaderResourceViewFromFile(d3d->GetDevice(),textureFiles[i],NULL,NULL,&m_textures[i],NULL);
		if (FAILED(result))
		{
			return false;
		}
	}
}

InstanceChannelList::InstanceChannelList()
{
	//set things to there defualt values
	m_channelMaxSize = 0;
	m_instanceBuffers = 0;

	m_isDirty = 0;
	m_needsRendered = 0;
	m_numberOfChannels = 0;
	m_textures = 0;
	m_head = NULL;
	m_tail = NULL;
}

bool InstanceChannelList::Initialize(bool * needsRendered, int * maxInstanceChannelSizes, int numberOfInstanceChannels, LPCWSTR * textures, D3DInterface * d3d)
{
	//store needs rendered list
	m_needsRendered = needsRendered;
	m_channelMaxSize = maxInstanceChannelSizes;
	m_numberOfChannels = numberOfInstanceChannels;

	m_isDirty = new bool[m_numberOfChannels];

	//set up the instance lists
	m_head = new Instance::InstanceNode*[m_numberOfChannels];
	m_tail = new Instance::InstanceNode*[m_numberOfChannels];

	//set up the counts
	m_counts = new int[m_numberOfChannels];

	//initialize counts to zero
	for (int i = 0; i <m_numberOfChannels; i++)
	{
		m_counts[i] = 0;
	}

	//initialize the is dirty to false
	ZeroMemory(m_isDirty, m_numberOfChannels);


	//load the textures
	if (!loadTextures(d3d, textures))
		return false;

	//load the buffers
	if (!setUpInstanceBuffers(d3d))
		return false;

	int test = 3;
	
}

void InstanceChannelList::Close()
{
	//cleaup memory
	
	//release all of the instance buffers
	for (int i = 0; i < m_numberOfChannels; i++)
	{
		m_instanceBuffers[i]->Release();

	}

	delete[] m_instanceBuffers;

	//delete the data lists
	//if instance data exists on a node then delete it
	for (int i = 0; i < m_numberOfChannels; i++)
	{
		Instance::InstanceNode* currentNode = m_head[0];
		for (int j = 0; j < m_counts[0]; j++)
		{
			//loop through all of the nodes and delete them
			Instance::InstanceNode* tempNode = currentNode->right;
			delete currentNode;
			currentNode = tempNode;
		}
	}

	//delete is dirty
	delete[] m_isDirty;

	//delete needs rendered
	delete[] m_needsRendered;

	//delete the channel max sizes
	delete[] m_channelMaxSize;

	//delete the count
	delete[] m_counts;

}

void InstanceChannelList::DeleteInstance(Instance* data, int channel)
{
	//delete the instance from the list
	if (m_head[channel] == data->GetNode())
	{
		//since the data was the head make the node to the right of the data be the head
		m_head[channel] == data->GetNode()->right;
	}
	else if (m_tail[channel] == data->GetNode())
	{
		//since the data was the tail make the node to the left of the data be the tail(its fine if its null)
		m_tail[channel] == data->GetNode()->left;
	}
	else
	{
		//node for the passed in instance is not the head or the tail so make the left and right of the node reference each other
		data->GetNode()->left->right = data->GetNode()->right;
		data->GetNode()->right->left = data->GetNode()->left;
	}
	m_counts[channel]--;
	m_isDirty[channel] = true;
}

void InstanceChannelList::AddInstance( int channel, Instance * instance)
{
	//add the instance to the list
	//if the head is equal to null then make this instance node the head
	if (m_counts[channel ] == 0)
	{
		m_head[channel] = instance->GetNode();
	}
	else
	{
		//if the tail is null make this the tail and make the head right = tail
		if (m_counts[channel] == 1)
		{
			m_tail[channel] = instance->GetNode();
			m_head[channel]->right = m_tail[channel];
		}
		else
		{
			//make the right of the tail = the new node
			m_tail[channel]->right = instance->GetNode();
			m_tail[channel] = instance->GetNode();
		}
	}
	//increase the count
	m_counts[channel]++;
	m_isDirty[channel] = true;

}

ID3D11ShaderResourceView * InstanceChannelList::GetTexture(int channel)
{
	return m_textures[channel];
}

ID3D11Buffer * InstanceChannelList::GetInstanceBuffer(D3DInterface* d3d,int index)
{
	//if the buffer is dirty then update it
	if (m_isDirty[index])
	{
		//update the instance buffer
		updateInstanceBuffer(d3d, index);
	}

	//set the thing to not dirty
	//m_isDirty[index] = false;
	return m_instanceBuffers[index];
}

int InstanceChannelList::GetNumberOfInstanceChannels()  
{
	return m_numberOfChannels;
}

int InstanceChannelList::getListCount(int list)
{
	return m_counts[list];
}

bool InstanceChannelList::updateInstanceBuffer(D3DInterface * d3d, int channel)
{
	HRESULT result;
	//update the resource
	D3D11_MAPPED_SUBRESOURCE mappedData;
	BaseShader::BaseShaderInstance* instanceData;
	int count = m_counts[channel]; 
	//map the instance data
	result = d3d->GetImmediateContext()->Map(m_instanceBuffers[channel], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	if (FAILED(result))
	{
		return false;
	}

	instanceData = (BaseShader::BaseShaderInstance*)mappedData.pData;
	Instance::InstanceNode* node = m_head[channel];
	//copy all of the instance data into the channel
	for (int i = 0; i < count; i++)
	{
		instanceData[i] = *node->data->GetInstanceData();
		node = node->right;
	}

	//un map the data
    d3d->GetImmediateContext()->Unmap(m_instanceBuffers[channel],0);

	//delete the instance data
	//delete[] instanceData;
}

bool InstanceChannelList::setUpInstanceBuffers(D3DInterface * d3d)
{
	m_instanceBuffers = new ID3D11Buffer*[m_numberOfChannels];
	//set up all of the instance buffers
	for (int i = 0; i < m_numberOfChannels; i++)
	{
		if (!setUpInstanceBuffer(d3d, i, m_channelMaxSize[i]))
			return false;
	}
}

bool InstanceChannelList::setUpInstanceBuffer(D3DInterface * d3d, int channel, int maxSize)
{
	//set up the specific instance buffer
	D3D11_BUFFER_DESC desc;
	HRESULT result;
	//zero memory for the desc
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeof(BaseShader::BaseShaderInstance)*maxSize;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	//create the instance buffer
	result = d3d->GetDevice()->CreateBuffer(&desc, NULL, &m_instanceBuffers[channel]);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
