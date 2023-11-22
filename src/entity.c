#include "entity.h"
#include "level.h"

#include "raymath.h"
#include "stdlib.h"
#include "cJSON.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "stdlib.h"
#include <stdio.h>

Entity* CreateEntity(Vector2 position, char* tag, bool isAffectedByGravity, float mass, Vector2 size, float drag, int uuid) {
    Entity* entity = malloc(sizeof(Entity));

    char *path = "resources/data/entity_tags.json";
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

    cJSON *tags_json = cJSON_Parse(dataBuffer);

    const cJSON *standable = cJSON_GetObjectItem(cJSON_GetObjectItem(tags_json, tag)->child, "standable");

    cJSON_Delete(tags_json);

    int stand = 0;

    entity[0] = (Entity)
    {
        .position = position,
        .isAffectedByGravity = isAffectedByGravity,
        .mass = mass,
        .size = size, 
        .drag = drag,

        .velocity = (Vector2){0, 0},
        .acceleration = (Vector2) {0, 0},
        .onGround = false,


        .hitbox = (Hitbox)
        {
            .rect = (Rectangle)
            {
                .x = position.x - size.x / 2,
                .y = position.y - size.y / 2,
                .width = size.x,
                .height = size.y
            },
            .tag = tag
        },

        .uuid = uuid,
        .standable = standable->valueint
    };
    
    return entity;
}

void UpdateEntity(Entity* entity, float gravity, float deltaTime)
{
    if(!entity->onGround && entity->isAffectedByGravity)
    {
        entity->velocity.y += gravity * deltaTime * 60;
    }

    entity->velocity = Vector2Add(entity->velocity, Vector2Scale(entity->acceleration, deltaTime * 60));
    
    entity->velocity.x -= entity->velocity.x * entity->drag;

    entity->position = Vector2Add(entity->position, Vector2Scale(entity->velocity, deltaTime * 60));

    entity->hitbox.rect = (Rectangle) {
        .x = entity->position.x - entity->size.x/2,
        .y = entity->position.y - entity->size.y/2,
        .width = entity->size.x,
        .height = entity->size.y
    };
}

void DrawEntity(Entity entity)
{
    DrawRectangle(entity.position.x - (entity.size.x / 2), entity.position.y - (entity.size.y / 2), entity.size.x, entity.size.y, RED);
}