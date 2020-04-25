#include "Entity.h"
#include "Definitions.h"
#include <cstdint>

using namespace std;

Entity::Entity(const uint16_t spriteIndex, const uint16_t imageIndex, const uint8_t chunkX, const uint8_t chunkY, const uint16_t x, const uint16_t y)
    : spriteIndex(spriteIndex)
    , imageIndex(imageIndex)
{
    currentChunkCoord[0] = chunkX;
    currentChunkCoord[1] = chunkY;
    position[0] = x;
    position[1] = y;
}

Entity::~Entity()
{
   //Implementar assim que a classe chunk estiver pronta 
}

uint8_t Entity::getCurrentChunkX()
{
    return currentChunkCoord[0];
}

uint8_t Entity::getCurrentChunkY()
{
    return currentChunkCoord[1];
}

uint8_t Entity::getImageIndex()
{
    return imageIndex;
}

uint8_t Entity::getEntityID()
{
    return entityID;
}

uint16_t Entity::getSpriteIndex()
{
    return spriteIndex;
}

uint16_t Entity::getX()
{
    return position[0];
}

uint16_t Entity::getY()
{
    return position[1];
}

void Entity::joinChunk(const uint8_t chunkX, const uint8_t chunkY)
{
   //Implementar assim que a classe chunk estiver pronta  
}