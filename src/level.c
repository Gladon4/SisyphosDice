#include "level.h"

#include "raylib.h"
#include "jsense.h"
#include <stdio.h>
#include <string.h>

Level CreateLevel(char* path, char* name, int chunkSize)
{
    JSENSE *json = jse_from_file(path);

    char *texture_path = jse_get(json, "texture");
    float gravity = atof(jse_get(json, "gravity"));
    float brightness = atof(jse_get(json, "brightness"));

    int width = atoi(jse_get(json, "size.[0]"));
    int height = atoi(jse_get(json, "size.[1]"));

    int i = 0;
    char query[30];

    int numberOfChunks = (int)(width / chunkSize);
    Chunk* chunks = (Chunk*)calloc(numberOfChunks, sizeof(Chunk));
    int* numberOfLevelHitboxesInChunks = (int*)calloc(numberOfChunks, sizeof(int));

    sprintf(query, "hitboxes.[%d]", i);

    while (jse_get(json, query))
    {
        sprintf(query, "hitboxes.[%d].x", i);
        int x = tec_string_to_int(jse_get(json, query));

        sprintf(query, "hitboxes.[%d].width", i);
        int width = tec_string_to_int(jse_get(json, query));
        
        int startingChunk = (int)(x / chunkSize);
        for (int j=startingChunk; j*chunkSize<x+width; j++) 
        {
            numberOfLevelHitboxesInChunks[j] += 1;
        }

        i++;
        sprintf(query, "hitboxes.[%d]", i);
    }

    for (int j=0; j<numberOfChunks; j++) 
    {
        if (numberOfLevelHitboxesInChunks[j] == 0)
        {
            continue;
        }

        chunks[j].numberOfLevelHitboxes = 0;
        chunks[j].levelHitboxesInChunk = calloc(numberOfLevelHitboxesInChunks[j], sizeof(Hitbox));
        // printf("\n Number of Hitboxes in Chunk %d: %d", j, numberOfLevelHitboxesInChunks[j]);
    }
    // printf("\n");


    for(int j = 0; j < i; j++)
    {
        sprintf(query, "hitboxes.[%d].x", j);
        int x = tec_string_to_int(jse_get(json, query));

        sprintf(query, "hitboxes.[%d].y", j);
        int y = tec_string_to_int(jse_get(json, query));

        sprintf(query, "hitboxes.[%d].width", j);
        int width = tec_string_to_int(jse_get(json, query));

        sprintf(query, "hitboxes.[%d].height", j);
        int height = tec_string_to_int(jse_get(json, query));

        sprintf(query, "hitboxes.[%d].tag", j);
        char* tag = jse_get(json, query);

        Hitbox hitbox = {
            .rect = (Rectangle) {
                .x = x,
                .y = y,
                .width = width,
                .height = height
            },
            .tag = tag
        };

        int startingChunk = (int)(x / chunkSize);
        for (int k=startingChunk; k*chunkSize<x+width; k++) 
        {
            chunks[k].levelHitboxesInChunk[chunks[k].numberOfLevelHitboxes] = hitbox;
            chunks[k].numberOfLevelHitboxes++;
        }
    }

    return (Level)
    {
        .chunkSize = chunkSize,
        .texture = LoadTexture(texture_path),
        .gravity = gravity,
        .name = name,
        .brightness = brightness,
        .size = (Vector2){width, height},
        .chunks = chunks,
        .numberOfChunks = numberOfChunks
    };
}

void DrawLevel(Level level)
{
    DrawTexture(level.texture, 0, 0, WHITE);
}

void DrawHitboxes(Level level)
{
    for (int i=0; i<level.numberOfChunks; i++) {
        for (int j=0; j<level.chunks[i].numberOfLevelHitboxes; j++)
        {
            Color color = BLUE;
            if (!strcmp(level.chunks[i].levelHitboxesInChunk[j].tag, "jump-through")) {color = GREEN;}

            Rectangle rect = level.chunks[i].levelHitboxesInChunk[j].rect;

            DrawRectangleLinesEx(rect, 2, color);
        } 
    }
}

void DrawChunksBorders(Level level)
{
    for (int i=0; i<level.numberOfChunks; i++)
    {
        DrawLine(i*level.chunkSize, 0, i*level.chunkSize, level.size.y, PURPLE);
    }
}