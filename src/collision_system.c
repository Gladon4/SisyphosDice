#include "collision_system.h"
#include "raymath.h"
#include "string.h"
#include "misc_math.h"
#include "stdio.h"

bool _PushEntity(Entity* pusher, Entity* pushee);

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
    if (!CheckCollisionRecs(entity1->hitbox.rect, entity2->hitbox.rect)) {
        return;
    }

    if (EntityHasTag(entity2, "pushable"))
    {
        if (_PushEntity(entity1, entity2))
        {
            entity1->state = pushing;
            return;
        }
    }

    Rectangle collisionRectangle = GetCollisionRec(entity1->hitbox.rect, entity2->hitbox.rect);
    bool fromAbove = entity2->hitbox.rect.y == collisionRectangle.y && collisionRectangle.width > collisionRectangle.height;

    if (EntityHasTag(entity2, "standable") && fromAbove)
    {
        entity1->acceleration.y = min(entity1->acceleration.y, 0);
        entity1->velocity.y = min(entity1->velocity.y, 0);
        entity1->position.y -= collisionRectangle.height;
    }

    return;

    /* Rectangle collisionRectangle = GetCollisionRec(entity1->hitbox.rect, entity2->hitbox.rect);

    if (entity1->onGround) {return;}
    return CollisionPreventionEntityHitbox(entity1, entity2->hitbox);

    if (!CheckCollisionRecs(entity1->hitbox.rect, entity2->hitbox.rect)) {
        return;
    } */

}

bool _PushEntity(Entity* pusher, Entity* pushee)
{
    Rectangle collisionRectangle = GetCollisionRec(pusher->hitbox.rect, pushee->hitbox.rect);

    if (collisionRectangle.width > collisionRectangle.height)
    {
        // can you push from above or below? probably not
        return false;
    }
    else
    {
        float weightFactor = pusher->mass / pushee->mass;
        weightFactor = Clamp(weightFactor, 0, 1);

        pusher->velocity.x = pusher->velocity.x * weightFactor;        
        
        bool fromLeft = pushee->hitbox.rect.x == collisionRectangle.x;

        if (fromLeft) 
        {
            pushee->position.x += collisionRectangle.width * 0.5;
            pusher->position.x -= collisionRectangle.width * 0.5;
        } 
        else
        {
            pushee->position.x -= collisionRectangle.width * 0.5;
            pusher->position.x += collisionRectangle.width * 0.5;
        }

        return true;
    }   
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