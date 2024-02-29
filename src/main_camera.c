#include "main_camera.h"
#include "raymath.h"

MainCamera CreateMainCamera(Vector2 startPosition, Vector2 size, Entity* target, float speed)
{
    Camera2D camera = { 0 };
    camera.target = (Vector2){ target->position.x, target->position.y};
    camera.offset = (Vector2){ size.x/2.0f, size.y/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    return (MainCamera) {
        .camera = camera,
        .position = startPosition,
        .size = size,
        .target = target,
        .speed = speed
    };
}


void UpdateCameraPosition(MainCamera* camera, Level level, float deltaTime) {
    Vector2 offset;

    offset = Vector2Subtract(camera->target->position, camera->position);
    offset = Vector2Scale(offset, deltaTime * camera->speed);

    camera->position = Vector2Add(camera->position, offset);
    camera->position = Vector2Clamp(camera->position, Vector2Add(Vector2Zero(), camera->camera.offset), Vector2Subtract(level.size, camera->camera.offset));

    camera->camera.target = (Vector2){ camera->position.x, camera->position.y };

}