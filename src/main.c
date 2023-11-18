#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "level.h"
#include "entity.h"
#include "collision_system.h"
#include "camera.h"


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
    Entity player = CreateEntity((Vector2){250, 50}, "player", false, 1, (Vector2){50, 50}, 0);
    MainCamera mainCamera = CreateCamera(player.position, screeSize, &player, 5);

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

        KeyBoardInput(&player);

        float deltaTime = GetFrameTime();

        Vector2 startPosition = player.position;
        UpdateEntity(&player, testLevel.gravity, deltaTime);
        Vector2 endPosition = player.position;      

        int startChunk;
        int endChunk;

        if (endPosition.x < startPosition.x) 
        {
            startChunk = (int)(endPosition.x - player.size.x/2)/testLevel.chunkSize;
            endChunk = (int)(startPosition.x + player.size.x/2)/testLevel.chunkSize;
        }
        else
        {
            startChunk = (int)(startPosition.x - player.size.x/2)/testLevel.chunkSize;
            endChunk = (int)(endPosition.x + player.size.x/2)/testLevel.chunkSize;
        }

        player.onGround = false;
        for (int i=startChunk; i<=endChunk; i++)
        {
            for (int j=0; j<testLevel.chunks[i].numberOfLevelHitboxes; j++)
            {
                CollisionPreventionEntityHitbox(&player, testLevel.chunks[i].levelHitboxesInChunk[j]);
                if (CheckForOnGround(player, testLevel.chunks[i].levelHitboxesInChunk[j])) 
                {
                    player.onGround = true;
                }
            }
        }

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

        DrawEntity(player, cameraDrawPosition);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}