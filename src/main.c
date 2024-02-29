#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "entity_manager.h"
#include "collision_system.h"
#include "level.h"
#include "entity.h"
#include "main_camera.h"


void KeyBoardInput(Entity* player, float deltaTime)
{
    if (IsKeyDown(KEY_A)) {player->velocity.x -= 1.2 * deltaTime * 60;}
    if (IsKeyDown(KEY_D)) {player->velocity.x += 1.2 * deltaTime * 60;}

    if (IsKeyDown(KEY_SPACE) && player->onGround) 
    {
        player->velocity.y = -25;
        player->onGround = false;
    } 
}

int main()
{
    Vector2 screeSize = (Vector2){900, 800};

    InitWindow(screeSize.x, screeSize.y, "Test Window");


    Level testLevel = CreateLevel("resources/levels/testStage.json", "Test Stage", 500);
    EntityManager entityManager = CreateEntityManager(testLevel, 1000);
    
    Entity* player = EntityManagerCreateEntity(&entityManager, (Vector2){400, 50}, "player", true, 1, (Vector2){50, 50}, 0.1);
    EntityManagerCreateEntity(&entityManager, (Vector2){200, 50}, "dice", true, 1.2, (Vector2){150, 150}, 0.1);
    EntityManagerCreateEntity(&entityManager, (Vector2){500, 50}, "test", true, 1.2, (Vector2){150, 150}, 0.1);

    MainCamera mainCamera = CreateMainCamera(Vector2Zero(), screeSize, player, 5);


    SetTargetFPS(60);
    bool showFPS = false;
    bool showHitboxes = false;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F1)) {showFPS = !showFPS;}
        if (IsKeyPressed(KEY_F2)) {showHitboxes = !showHitboxes;}
        if (IsKeyPressed(KEY_F8)) {SetTargetFPS(60);}
        if (IsKeyPressed(KEY_F9)) {SetTargetFPS(120);}
        if (IsKeyPressed(KEY_F10)) {SetTargetFPS(240);}
        if (IsKeyPressed(KEY_F11)) {SetTargetFPS(0);}        


        float deltaTime = GetFrameTime();

        UpdateEntities(entityManager, testLevel.gravity, deltaTime);
        UpdateCameraPosition(&mainCamera, testLevel, deltaTime);
        KeyBoardInput(player, deltaTime); 

        BeginDrawing();
        BeginMode2D(mainCamera.camera);
        
        ClearBackground(RAYWHITE);
        DrawLevel(testLevel);

        DrawEntities(entityManager);
        
        if (showHitboxes) {
            DrawHitboxes(testLevel);
            DrawChunksBorders(testLevel);
        }

        EndMode2D();
        
        if (showFPS) {DrawFPS(10, 10);}
        
        EndDrawing();

    }

    CloseWindow();

    return 0;
}
