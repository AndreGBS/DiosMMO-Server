#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

using namespace std;

class Entity
{
public:
    Entity(const uint16_t spriteIndex, const uint16_t imageIndex, const uint8_t chunkX, const uint8_t chunkY, const uint16_t x, const uint16_t y);
    ~Entity();
    uint8_t getImageIndex();
    uint8_t getCurrentChunkX();
    uint8_t getCurrentChunkY();
    uint8_t getEntityID();
    uint16_t getX();
    uint16_t getY();
    uint16_t getSpriteIndex();
    
private:
    uint8_t entityID;
    uint8_t imageIndex;
    uint8_t currentChunkCoord[2];
    uint16_t spriteIndex;
    uint16_t position[2];
    
    void joinChunk(const uint8_t chunkX, const uint8_t chunkY);

    friend class Chunk;
 };

#endif