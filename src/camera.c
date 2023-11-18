#include "camera.h"
#include "raymath.h"

MainCamera CreateCamera(Vector2 startPosition, Vector2 size, Entity* target, float speed)
{
    return (MainCamera) {
        .position = startPosition,
        .size = size,
        .target = target,
        .speed = speed
    };
}


void UpdateCameraPosition(MainCamera* camera, Level level, float deltaTime) {
    Vector2 offset;
    Vector2 screenOffset;

    offset = Vector2Subtract(camera->target->position, camera->position);
    offset = Vector2Scale(offset, deltaTime * camera->speed);

    screenOffset = Vector2Scale(camera->size, 0.5);

    camera->position = Vector2Add(camera->position, offset);
    camera->position = Vector2Clamp(camera->position, Vector2Add(Vector2Zero(), screenOffset), Vector2Subtract(level.size, screenOffset));
}