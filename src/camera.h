#ifndef CAMERA_H_
#define CAMERA_H_

#include "raylib.h"
#include "entity.h"
#include "level.h"

typedef struct MainCamera
{
    Vector2 position;
    Vector2 size;
    Entity* target;
    float speed;
} MainCamera;

MainCamera CreateCamera(Vector2 startPosition, Vector2 size, Entity* target, float speed);

void UpdateCameraPosition(MainCamera* camera, Level level, float deltaTime);

#endif