#include "collision_system.h"
#include "raymath.h"
#include "string.h"
#include "misc_math.h"

void CollisionPreventionEntityHitbox(Entity* entity, Hitbox hitbox)
{
    if (!CheckCollisionRecs(entity->hitbox.rect, hitbox.rect)) {
        return;
    }

    Rectangle collisionRectangle = GetCollisionRec(entity->hitbox.rect, hitbox.rect);

    if (!strcmp(hitbox.tag, "jump-through"))
    {
        bool fromAbove = hitbox.rect.y == collisionRectangle.y;
        if (!fromAbove)
        {
            entity->velocity.y = -30;
            return;
        }
    }

    if (collisionRectangle.width > collisionRectangle.height)
    {
        bool fromAbove = hitbox.rect.y == collisionRectangle.y;

        if (fromAbove) 
        {
            entity->acceleration.y = min(entity->acceleration.y, 0);
            entity->velocity.y = min(entity->velocity.y, 0);
            entity->position.y -= collisionRectangle.height;
        } 
        else
        {
            entity->acceleration.y = max(entity->acceleration.y, 0);
            entity->velocity.y = max(entity->velocity.y, 0);
            entity->position.y += collisionRectangle.height;
        }
    }
    else
    {
        bool fromLeft = hitbox.rect.x == collisionRectangle.x;

        if (fromLeft) 
        {
            entity->acceleration.x = min(entity->acceleration.x, 0);
            entity->velocity.x = min(entity->velocity.x, 0);
            entity->position.x -= collisionRectangle.width;
        } 
        else
        {
            entity->acceleration.x = max(entity->acceleration.x, 0);
            entity->velocity.x = max(entity->velocity.x, 0);
            entity->position.x += collisionRectangle.width;
        }
    }
}

void CollisionPreventionEntityEntity(Entity* entity1, Entity* entity2)
{
    if (entity1->onGround) {return;}
    return CollisionPreventionEntityHitbox(entity1, entity2->hitbox);

    if (!CheckCollisionRecs(entity1->hitbox.rect, entity2->hitbox.rect)) {
        return;
    }

    Rectangle collisionRectangle = GetCollisionRec(entity1->hitbox.rect, entity2->hitbox.rect);

    
}

bool CheckForOnGround(Entity entity, Hitbox hitbox)
{
    Rectangle rectCast = (Rectangle) {
        .x = entity.position.x - entity.size.x/2 + 2,
        .y = entity.position.y + entity.size.y/2,
        .width = entity.size.x - 2,
        .height = 1
    };

    if (CheckCollisionRecs(rectCast, hitbox.rect)) {
        return true;
    } 

    return false;
}