#include "entity_manager.h"
#include "stdlib.h"

EntityManager CreateEntityManager(Level level)
{
    return (EntityManager) {
        .entities = calloc(100, sizeof(Entity*)),
        .numberOfEntities = 0,
        .currentUUID = 0,
        .level = level
    };
}

Entity* EntityManagerCreateEntity(EntityManager* entityManager, Vector2 position, char* tag, bool isAffectedByGravity, float mass, Vector2 size, float drag)
{
    Entity* entity = CreateEntity(position, tag, isAffectedByGravity, mass, size, drag, entityManager->currentUUID);
    
    entityManager->entities[entityManager->numberOfEntities] = entity;
    entityManager->numberOfEntities++;
    entityManager->currentUUID++;

    return entity;
}

void AddEntityToEntityManager(EntityManager* entityManager, Entity* entity) 
{
    entityManager->entities[entityManager->numberOfEntities] = entity;
    entityManager->numberOfEntities++;
}

void RemoveEntityFromEntityManager(EntityManager* entityManager, Entity* entity)
{
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        if (entityManager->entities[i]->uuid == entity->uuid)
        {
            entityManager->entities[i] = entityManager->entities[entityManager->numberOfEntities-1];
            entityManager->numberOfEntities--;
            return;
        }
    }
}

void UpdateEntities(EntityManager entityManager, float gravity, float deltaTime)
{
    for (int i=0; i<entityManager.numberOfEntities; i++)
    {
        UpdateEntity(entityManager.entities[i], gravity, deltaTime);
        _CollisionPreventionEntityLevelHitbox(entityManager.entities[i], entityManager.level);
        _CollisionPreventionEntityEntity(entityManager.entities[i], entityManager.level);
    }
}


void _CollisionPreventionEntityLevelHitbox (Entity* entity, Level level)
{

}

void _CollisionPreventionEntityEntity (Entity* entity, Level level)
{

}

void _UpdateEntityChunk (Entity* entity, Level level)
{
    
}


void DrawEntities(EntityManager entityManager, Vector2 position)
{
    for (int i=0; i<entityManager.numberOfEntities; i++)
    {
        DrawEntity(*entityManager.entities[i], position);
    }
}