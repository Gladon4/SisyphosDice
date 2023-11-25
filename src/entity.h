#ifndef ENTITY_H_
#define ENTITY_H_

#include "raylib.h"
#include "hitbox.h"

typedef enum STATE
{
    idle,
    pushing
} STATE;

typedef struct Entity
{
    float mass;
    Vector2 size;
    float drag;

    Hitbox hitbox;

    bool isAffectedByGravity;
    bool onGround;

    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    int uuid;
    char* tags;
    STATE state;
} Entity;

Entity* CreateEntity(Vector2 position, char* tag, bool isAffectedByGravity, float mass, Vector2 size, float drag, int uuid);

void UpdateEntity(Entity* entity, float gravity, float deltaTime);
void UpdateEntityHitBox(Entity* entity);

void DrawEntity(Entity entity);

bool EntityHasTag(Entity* entity, char* tag);

#endif
