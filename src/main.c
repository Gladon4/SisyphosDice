#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "level.h"
#include "entity.h"
#include "collision_system.h"
#include "camera.h"
#include "entity_manager.h"


void KeyBoardInput(Entity* player)
{
    if (IsKeyDown(KEY_A)) {player->velocity.x = -10;}
    else if (IsKeyDown(KEY_D)) {player->velocity.x = 10;}
    else {player->velocity.x = 0;}

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
    EntityManager entityManager = CreateEntityManager(testLevel);
    
    char tag[] = "player";
    Entity* player = EntityManagerCreateEntity(&entityManager, (Vector2){250, 50}, tag, false, 1, (Vector2){50, 50}, 0);
    
    MainCamera mainCamera = CreateCamera(player->position, screeSize, player, 5);
    

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

        KeyBoardInput(player);

        float deltaTime = GetFrameTime();

        UpdateEntities(entityManager, testLevel.gravity, deltaTime);
        UpdateCameraPosition(&mainCamera, testLevel, deltaTime);
        Vector2 cameraDrawPosition = Vector2Subtract(Vector2Scale(mainCamera.size, 0.5), mainCamera.position);

        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawLevel(testLevel, cameraDrawPosition);

        if (showFPS) {DrawFPS(10, 10);}
        if (showHitboxes) {
            DrawHitboxes(testLevel, cameraDrawPosition);
            DrawChunksBorders(testLevel,cameraDrawPosition);
        }

        DrawEntities(entityManager, cameraDrawPosition);

        EndDrawing();

    }

    CloseWindow();

    return 0;
}