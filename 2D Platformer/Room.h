#pragma once
#include "InstanceChannelList.h"
#include <string>
//this class is used to load tile sets and enemys and let the player check for collisions with them
class Room
{
private:
	struct Tile{ 
		unsigned char collisionMask;
		float slopeAdd;
		float slope;
		unsigned char textureID;
		Tile(unsigned char collisionMask = 0, float slopAdd = 0.0f, float slope =1 , unsigned char textureOffsetId =0);
	};



public:
	Room();

	bool Initialize(char* Level);

	bool Collides(float* x, float* y, float halfWidth, float halfHeight,const unsigned char* collsionMasks);

	unsigned char GetTextureID();

	bool GenerateBuffer(InstanceChannelList buffer);

private:
	void readTile(std::istream stream);
	
private:

	int m_numberOfTiles;
	int** m_tiles;

	unsigned char m_textureID;

	Tile* m_tileTemplates;

	float m_width;

	float m_height;

};