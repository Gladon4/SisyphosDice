#ifndef LEVEL_H_
#define LEVEL_H_

#include "raylib.h"
#include "hitbox.h"
#include "chunk.h"

typedef struct Level
{
    int chunkSize;
    char* name;
    Texture2D texture;
    float gravity;
    Vector2 size;
    float brightness;
    Chunk* chunks;
    int numberOfChunks;

} Level;



Level CreateLevel(char* path, char* name, int chunkSize);

void DrawLevel(Level level);

void DrawHitboxes(Level level);

void DrawChunksBorders(Level level);

#endif
