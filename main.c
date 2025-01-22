#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "InitialGame.h"
#include "GameUpdate.h"

// Program main entry point
int main() {

    InitWindow(800, 200, "starting window");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        int TextSize = MeasureText("Press N to start a new game", 20);
        DrawText("Press N to start a new game\nPress L to load the last game\nPress D for default map",
                 800 / 2 - TextSize / 2, 200 / 2 - 25, 20, GREEN);
        ClearBackground(BLACK);
        if (IsKeyPressed(KEY_N)) {
            initialMapMaker();
            makeKingdom();
            makeVillage();
            makeBarrier();
            break;
        }
        if (IsKeyPressed(KEY_L)) {
            FILE *fileReader;
            fileReader = fopen("SavedGame.dat", "rb");
            if (!fileReader) {
                printf("Can't load the last game. pLease start a new one\n");
                initialMapMaker();
                makeKingdom();
                makeVillage();
                makeBarrier();
            } else {
                gameState lastGame;
                fread(&lastGame, sizeof(gameState), 1, fileReader);
                LoadGame(&lastGame);
                fread(&isPlayingWithMonte, sizeof(int), 1, fileReader);
                fread(&iterations, sizeof(int), 1, fileReader);
            }
            fclose(fileReader);
            break;
        }
        if (IsKeyPressed(KEY_D)) {
            FILE *fileReader;
            fileReader = fopen("..\\Default.dat", "rb");
            if (!fileReader) {
                printf("Can't load the default map. pLease start a new one\n");
                initialMapMaker();
                makeKingdom();
                makeVillage();
                makeBarrier();
            } else {
                gameState DefaultGame;
                fread(&DefaultGame, sizeof(gameState), 1, fileReader);
                LoadGame(&DefaultGame);
                isPlayingWithMonte = 0;
            }
            fclose(fileReader);
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    action = 0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "main game window");

    Vector2 mapCenter = {TILE_SIZE * 12, TILE_SIZE * 11};
    map0 = (Vector2) {mapCenter.x - (mapWidth * TILE_SIZE / 2), mapCenter.y - (mapHeight * TILE_SIZE / 2)};

    if (!isPlayingWithMonte) buttonsPosY = -100;

    Texture2D mapTileSet = LoadTexture("..\\assets\\initial map (1).png");
    Texture2D GroundTile = LoadTexture("..\\assets\\tilecrop (1).png");
    Texture2D Castle = LoadTexture("..\\assets\\stone castle.png");
    Texture2D Stone = LoadTexture("..\\assets\\stone2.png");
    Texture2D House = LoadTexture("..\\assets\\house 2.png");
    Texture2D roadMan = LoadTexture("..\\assets\\SteamMan_attack1.png");
    Texture2D explosion = LoadTexture("..\\assets\\explosion.png");
    Texture2D quoteBox = LoadTexture("..\\assets\\Quote-box.png");

    Font font = LoadFont("..\\assets\\pixantiqua.png");
    Font myFont = LoadFont("..\\\\SpaceMono-Bold.ttf");

    kingdoms[0].color = WHITE;
    if (!kingdoms[1].dead)
        kingdoms[1].color = (Color) {103, 135, 194, 255};
    if (!kingdoms[2].dead)
        kingdoms[2].color = (Color) {230, 37, 82, 255};
    if (!kingdoms[3].dead)
        kingdoms[3].color = (Color) {255, 114, 43, 255};
    if (!kingdoms[4].dead)
        kingdoms[4].color = (Color) {189, 125, 219, 255};

    SetTargetFPS(60);
    int manIndex = 0;
    float manTimer = MAN_TIME;
    float flameTimer = FLAME_TIME;
    int AnimationCounter = 0;

    //Main game loop
    while (!WindowShouldClose()) {
        //Hover effect vectors
        mousePosition = GetMousePosition();
        coordination = (Vector2) {(int) ((mousePosition.x - map0.x) / TILE_SIZE),
                                  (int) ((mousePosition.y - map0.y) / TILE_SIZE)};

        BeginDrawing();

        ClearBackground(BLACK);

        mapDrawer(mapTileSet, GroundTile, Castle, House, Stone, quoteBox, font, myFont);

        for (int i = 0; i < kingdomNumber; ++i) { //show kingdoms' info
            char text[50];
            Color textColor;
            if (kingdoms[i + 1].dead)
                textColor = (Color) {30, 30, 30, 255};
            else if (i + 1 != turn)
                textColor = transparentWhite;
            else
                textColor = kingdoms[i + 1].color;
            sprintf(text, "Kingdom %d", i + 1);
            DrawText(text, SCREEN_WIDTH - 285, 15 + (SCREEN_HEIGHT / kingdomNumber) * i, 30, textColor);
            sprintf(text, "Food = %d    FoodX = %d", kingdoms[i + 1].food, kingdoms[i + 1].foodX);
            DrawText(text, SCREEN_WIDTH - 285, 50 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Gold = %d    GoldX = %d", kingdoms[i + 1].gold, kingdoms[i + 1].goldX);
            DrawText(text, SCREEN_WIDTH - 285, 75 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Soldiers = %d", kingdoms[i + 1].soldier);
            DrawText(text, SCREEN_WIDTH - 285, 100 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Workers = %d", kingdoms[i + 1].worker);
            DrawText(text, SCREEN_WIDTH - 285, 125 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Villages = %d", kingdoms[i + 1].villageNumber);
            DrawText(text, SCREEN_WIDTH - 285, 150 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
        }


        if (mode == 0) //start of the turn
            mode0();

        if (mode == 1)  //actions
            mode1();

        if (mode == 2) { //making roads
            if (IsKeyPressed(KEY_B))
                mode = 0;
            kingdoms[turn].availableNumber = 0;
            int check = checkNeighbors(kingdoms[turn].x, kingdoms[turn].y);

            for (int i = 0; i < kingdoms[turn].roadNumber && check == 0; ++i) {
                check = checkNeighbors(kingdoms[turn].road[i].x, kingdoms[turn].road[i].y);
            }

            for (int i = 0; i < villageNumber && check == 0; i++) {
                if (villages[i].kingdom == turn) {
                    check = checkNeighbors(villages[i].x, villages[i].y);
                }
            }
            for (int i = 0; i < kingdoms[turn].availableNumber; ++i) {
                if (checkForWar(kingdoms[turn].available[i].x, kingdoms[turn].available[i].y)) {
                    DrawRectangle(kingdoms[turn].available[i].x * TILE_SIZE + map0.x,
                                  kingdoms[turn].available[i].y * TILE_SIZE + map0.y,
                                  TILE_SIZE, TILE_SIZE, transparentRed);
                } else {
                    DrawRectangle(kingdoms[turn].available[i].x * TILE_SIZE + map0.x,
                                  kingdoms[turn].available[i].y * TILE_SIZE + map0.y,
                                  TILE_SIZE, TILE_SIZE, transparentGreen);
                }
            }
        }

        if (mode == 3) //the end of the game
            mode3();

        if (mode == 4) { //animation
            if (animation == 1) { //making roads
                if (AnimationCounter < 12) {
                    manTimer -= GetFrameTime();
                    if (manTimer < 0) {
                        AnimationCounter++;
                        manTimer = MAN_TIME;
                        manIndex++;
                        if (manIndex >= MAN_COUNT) manIndex = 0;
                    }
                    Rectangle source = (Rectangle) {TILE_SIZE * manIndex, 0, TILE_SIZE, TILE_SIZE};
                    DrawTextureRec(roadMan, source, manPos, WHITE);
                } else {
                    RoadMaker();
                    AnimationCounter = 0;
                }
            } else if (animation == 2) { //all-out war
                if (AnimationCounter < 28) {
                    flameTimer -= GetFrameTime();
                    if (flameTimer < 0) {
                        AnimationCounter++;
                        flameTimer = FLAME_TIME;
                        manIndex++;
                        if (manIndex >= FLAME_COUNT) manIndex = 0;
                    }
                    manPos = (Vector2) {(kingdoms[toBeDeleted].x - .3) * TILE_SIZE + map0.x,
                                        (kingdoms[toBeDeleted].y - .4) * TILE_SIZE + map0.y};
                    Rectangle source = (Rectangle) {FLAME_WIDTH * manIndex, 0, FLAME_WIDTH, FLAME_HEIGHT};
                    DrawTextureRec(explosion, source, manPos, WHITE);
                } else {
                    AnimationCounter = 0;
                    DeleteKingdom(toBeDeleted);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();

    InitWindow(500, 200, "quit screen");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        char text[50] = "Do you want to save the game? (Y/N)";
        int textWidth = MeasureText(text, 20);
        DrawText(text, (500 - textWidth) / 2, 90, 20, GREEN);
        if (IsKeyPressed(KEY_Y)) {
            gameState lastGame;
            SaveGame(&lastGame);
            FILE *fileWriter;
            fileWriter = fopen("SavedGame.dat", "wb");
            if (!fileWriter) {
                strcpy(text, "Can't save the game. SORRYYYYY T-T\n");
                DrawText(text, (500 - textWidth) / 2, 90, 20, GREEN);
            } else {
                fwrite(&lastGame, sizeof(gameState), 1, fileWriter);
                fwrite(&isPlayingWithMonte, sizeof(int), 1, fileWriter);
                fwrite(&iterations, sizeof(int), 1, fileWriter);
                fclose(fileWriter);
                break;
            }
        }
        if (IsKeyPressed(KEY_N)) {
            break;
        }
        EndDrawing();
    }


    // De-Initialization
    UnloadTexture(mapTileSet);
    UnloadTexture(GroundTile);
    UnloadTexture(Stone);
    UnloadTexture(House);
    UnloadTexture(Castle);
    UnloadTexture(roadMan);
    UnloadTexture(explosion);
    UnloadFont(font);
    UnloadFont(myFont);
    CloseWindow();

    return 0;
}
