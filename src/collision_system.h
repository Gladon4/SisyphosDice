#ifndef COLLISION_SYSTEM_H_
#define COLLISION_SYSTEM_H_

#include "raylib.h"
#include "hitbox.h"
#include "entity.h"

void CollisionPreventionEntityHitbox(Entity* entity, Hitbox hitbox); 

void CollisionPreventionEntityEntity(Entity* entity1, Entity* entity2);

bool CheckForOnGround(Entity entity, Hitbox hitbox);

#endif