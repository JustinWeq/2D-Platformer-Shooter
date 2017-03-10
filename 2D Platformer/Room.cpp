#include "Room.h"

Room::Room()
{
	//set the defualt values
	m_height = 0;
	m_textureID = 0;
	m_tiles = NULL;
	m_tileTemplates = NULL;
	m_width = 0;
}

bool Room::Initialize(char * Level)
{
	std::ifstream stream = std::ifstream(Level);

	//if the stream does not exists return false
	if (!stream)
		return false;

	char* argument;
	stream >> argument;
	while (argument != NULL && argument != nullptr)
	{
		//set the argument to upper case
		TO_UPPER(argument);
		//read the argument
		if (argument == "TILE_DEFINITION")
		{
			//call the read tile function
		}
	}
}

bool Room::Collides(float * x, float * y, float halfWidth, float halfHeight, const unsigned char * collsionMasks)
{
	return false;
}

unsigned char Room::GetTextureID()
{
	return 0;
}

bool Room::GenerateBuffer(InstanceChannelList buffer)
{
	return false;
}

Room::Tile::Tile(unsigned char collisionMask, float slopAdd, float slope, unsigned char textureOffsetId)
{
}
