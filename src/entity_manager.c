#include "entity_manager.h"
#include "stdlib.h"
#include "string.h"
#include "chunk.h"
#include "collision_system.h"


void _CollisionPreventionEntityLevelHitbox (Chunk chunk);
void _CollisionPreventionEntityEntity (Chunk chunk);
void _UpdateEntityChunks (EntityManager entityManager);
bool _IsEntityStandable (Entity* entity);

EntityManager CreateEntityManager(Level level)
{
    return (EntityManager) {
        .entities = calloc(1000, sizeof(Entity*)),
        .numberOfEntities = 0,
        .currentUUID = 0,
        .level = level
    };
}

Entity* EntityManagerCreateEntity(EntityManager* entityManager, Vector2 position, char* tag, bool isAffectedByGravity, float mass, Vector2 size, float drag)
{
    char tagpointer[100];

    strncpy(tagpointer, tag, 100);

    Entity* entity = CreateEntity(position, tagpointer, isAffectedByGravity, mass, size, drag, entityManager->currentUUID);
    
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
        entityManager.entities[i]->onGround = false;
    }

    _UpdateEntityChunks(entityManager);
    
    for (int i=0; i<entityManager.level.numberOfChunks; i++)
    {
        _CollisionPreventionEntityLevelHitbox(entityManager.level.chunks[i]);
        _CollisionPreventionEntityEntity(entityManager.level.chunks[i]);
    }
}


void _CollisionPreventionEntityLevelHitbox (Chunk chunk)
{
    for (int i=0; i<chunk.numberOfEntities; i++)
    {
        for (int j=0; j<chunk.numberOfLevelHitboxes; j++)
        {
            CollisionPreventionEntityHitbox(chunk.entitiesInChunk[i], chunk.levelHitboxesInChunk[j]);
            if (CheckForOnGround(*chunk.entitiesInChunk[i], chunk.levelHitboxesInChunk[j])) 
            {
                chunk.entitiesInChunk[i]->onGround = true;
            }
        }
    }
}

void _CollisionPreventionEntityEntity (Chunk chunk)
{
    for (int i=0; i<chunk.numberOfEntities; i++)
    {
        for (int j=0; j<chunk.numberOfEntities; j++)
        {
            if (i == j) {continue;}
            
            CollisionPreventionEntityEntity(chunk.entitiesInChunk[i], chunk.entitiesInChunk[j]);
            if (CheckForOnGround(*chunk.entitiesInChunk[i], chunk.entitiesInChunk[j]->hitbox)) {
                chunk.entitiesInChunk[i]->onGround = true;
            }      
        }
    }
}

bool _IsEntityStandable (Entity* entity)
{
    if (entity->onGround)
    {
        return true;
    }
    
    return false;
}

void _UpdateEntityChunks (EntityManager entityManager)
{
    for (int i=0; i<entityManager.level.numberOfChunks; i++)
    {
        free(entityManager.level.chunks[i].entitiesInChunk); 
        entityManager.level.chunks[i].entitiesInChunk = calloc(1000, sizeof(Entity*));

        entityManager.level.chunks[i].numberOfEntities = 0;
    }
    
    for (int i=0; i<entityManager.numberOfEntities; i++)
    {
        int leftChunk = (int)(entityManager.entities[i]->position.x - entityManager.entities[i]->size.x/2)/entityManager.level.chunkSize;
        int rightChunk = (int)(entityManager.entities[i]->position.x + entityManager.entities[i]->size.x/2)/entityManager.level.chunkSize;
    
        entityManager.level.chunks[leftChunk].entitiesInChunk[entityManager.level.chunks[leftChunk].numberOfEntities] = entityManager.entities[i];
        entityManager.level.chunks[leftChunk].numberOfEntities++;
        
        if (rightChunk != leftChunk) {
            entityManager.level.chunks[rightChunk].entitiesInChunk[entityManager.level.chunks[rightChunk].numberOfEntities] = entityManager.entities[i];
            entityManager.level.chunks[rightChunk].numberOfEntities++;
        }
    }
}


void DrawEntities(EntityManager entityManager)
{
    for (int i=0; i<entityManager.numberOfEntities; i++)
    {
        DrawEntity(*entityManager.entities[i]);
    }
}