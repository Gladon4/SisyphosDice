#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include "raylib.h"
#include "entity.h"
#include "level.h"

typedef struct EntityManager
{
    Entity** entities;
    int numberOfEntities;
    int currentUUID;

    Level level;
} EntityManager;

EntityManager CreateEntityManager(Level level, const int maxNumberOfEntities);

Entity* EntityManagerCreateEntity(EntityManager* entityManager, Vector2 position, char* tag, bool isAffectedByGravity, float mass, Vector2 size, float drag);

void AddEntityToEntityManager(EntityManager* entityManager, Entity* entity);
void RemoveEntityFromEntityManager(EntityManager* entityManager, Entity* entity);

void UpdateEntities(EntityManager entityManager, float gravity, float deltaTime);
void DrawEntities(EntityManager entityManager);

#endif