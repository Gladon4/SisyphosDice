#ifndef CHUNK_H_
#define CHUNK_H_

#include "raylib.h"

#include "hitbox.h"
#include "entity.h"

typedef struct Chunk
{
    Hitbox* levelHitboxesInChunk;
    Entity** entitiesInChunk;
    int numberOfLevelHitboxes;
    int numberOfEntities;
} Chunk;


#endif
