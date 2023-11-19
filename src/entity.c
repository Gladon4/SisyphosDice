#include "entity.h"
#include "level.h"

#include "raymath.h"
#include "stdlib.h"

Entity* CreateEntity(Vector2 position, char* tag, bool isAffectedByGravity, float mass, Vector2 size, float drag, int uuid) {
    Entity* entity = malloc(sizeof(Entity));

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

        .uuid = uuid
    };
    
    return entity;
}

void UpdateEntity(Entity* entity, float gravity, float deltaTime)
{
    if(!entity->onGround)
    {
        entity->velocity.y += gravity * deltaTime * 60;
    }

    entity->velocity = Vector2Add(entity->velocity, Vector2Scale(entity->acceleration, deltaTime * 60));
    entity->position = Vector2Add(entity->position, Vector2Scale(entity->velocity, deltaTime * 60));

    entity->hitbox.rect = (Rectangle) {
        .x = entity->position.x - entity->size.x/2,
        .y = entity->position.y - entity->size.y/2,
        .width = entity->size.x,
        .height = entity->size.y
    };
}

void DrawEntity(Entity entity, Vector2 position)
{
    DrawRectangle(entity.position.x - (entity.size.x / 2) + position.x, entity.position.y - (entity.size.y / 2) + position.y, entity.size.x, entity.size.y, RED);
}