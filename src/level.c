#include "level.h"

#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "stdlib.h"


Level CreateLevel(char* path, char* name, int chunkSize)
{
    FILE *json_file_pointer = fopen(path, "r");
    if (json_file_pointer == NULL)
    {
        // laod error handling
    }

    struct stat fileStats; 

    if (stat(path, &fileStats) != 0)
    {
        // file stat error handling
    }

    int fileSize = fileStats.st_size;
    char dataBuffer[fileSize];
    fread(dataBuffer, 1, sizeof(dataBuffer), json_file_pointer); 

    fclose(json_file_pointer);

    
    cJSON *level_json = cJSON_Parse(dataBuffer);

    const cJSON *brightness = NULL;
    const cJSON *gravity = NULL;
    const cJSON *texture_path = NULL;
    const cJSON *size = NULL;
    
    const cJSON *hitboxes = NULL;

    brightness = cJSON_GetObjectItemCaseSensitive(level_json, "brightness");
    gravity = cJSON_GetObjectItemCaseSensitive(level_json, "gravity");
    texture_path = cJSON_GetObjectItemCaseSensitive(level_json, "texture");
    size = cJSON_GetObjectItemCaseSensitive(level_json, "size");

    Vector2 sizeVector = (Vector2) { cJSON_GetArrayItem(size, 0)->valuedouble, cJSON_GetArrayItem(size, 1)->valuedouble };
    
    hitboxes = cJSON_GetObjectItemCaseSensitive(level_json, "hitboxes");
    int numberOfHitboxes = cJSON_GetArraySize(hitboxes);

    int numberOfChunks = (int)(sizeVector.x / chunkSize);
    Chunk* chunks = (Chunk*)calloc(numberOfChunks, sizeof(Chunk));
    int* numberOfLevelHitboxesInChunks = (int*)calloc(numberOfChunks, sizeof(int));

    for (int i=0; i<numberOfHitboxes; i++)
    {
        cJSON *hitboxObject = cJSON_GetArrayItem(hitboxes, i);

        int x = cJSON_GetObjectItemCaseSensitive(hitboxObject, "x")->valueint;
        int width = cJSON_GetObjectItemCaseSensitive(hitboxObject, "width")->valueint;

        int startingChunk = (int)(x / chunkSize);
        for (int j=startingChunk; j*chunkSize<x+width; j++) 
        {
            numberOfLevelHitboxesInChunks[j] += 1;
        }
    }

    for (int i=0; i<numberOfChunks; i++) 
    {
        if (numberOfLevelHitboxesInChunks[0] == 0)
        {
            continue;
        }

        chunks[i].numberOfLevelHitboxes = 0;
        chunks[i].levelHitboxesInChunk = calloc(numberOfLevelHitboxesInChunks[i], sizeof(Hitbox));
    }

    for (int i=0; i<numberOfHitboxes; i++)
    {
        cJSON *hitboxObject = cJSON_GetArrayItem(hitboxes, i);

        int x = cJSON_GetObjectItemCaseSensitive(hitboxObject, "x")->valueint;
        int y = cJSON_GetObjectItemCaseSensitive(hitboxObject, "y")->valueint;
        int width = cJSON_GetObjectItemCaseSensitive(hitboxObject, "width")->valueint;
        int height = cJSON_GetObjectItemCaseSensitive(hitboxObject, "height")->valueint;
        int tag_len = strlen(cJSON_GetObjectItemCaseSensitive(hitboxObject, "tag")->valuestring);
        char *tag = cJSON_GetObjectItemCaseSensitive(hitboxObject, "tag")->valuestring;

        Hitbox hitbox = {
            .rect = (Rectangle) {
                .x = x,
                .y = y,
                .width = width,
                .height = height
            },
            .tag = (char*)malloc(tag_len)
        };

        strcpy(hitbox.tag, tag);

        int startingChunk = (int)(x / chunkSize);
        for (int j=startingChunk; j*chunkSize<x+width; j++) 
        {
            chunks[j].levelHitboxesInChunk[chunks[j].numberOfLevelHitboxes] = hitbox;
            chunks[j].numberOfLevelHitboxes++;
        }
    }

    Texture2D texture = LoadTexture(texture_path->valuestring);

    cJSON_Delete(level_json);

    return (Level)
    {
        .chunkSize = chunkSize,
        .brightness = brightness->valuedouble,
        .gravity = gravity->valuedouble,
        .texture = texture,
        .size = sizeVector,
        .name = name,
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