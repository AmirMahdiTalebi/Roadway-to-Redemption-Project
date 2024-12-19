#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "InitialMap.h"

struct button {
    Rectangle rect;
    char text[30];
    Color color;
};
typedef struct button button;
button buttons[5];

int winner;

// Program main entry point
int main() {
    int action = 0;
    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "starting window");

    Vector2 mapCenter= {TILE_SIZE*12, TILE_SIZE*11};
    Vector2 map0= {mapCenter.x - (mapWidth*TILE_SIZE/2), mapCenter.y - (mapHeight*TILE_SIZE/2)};

    int kingdomVertexNumber = kingdoms[turn].y*mapWidth + kingdoms[turn].x;
    for(int i=0; i<villageNumber; i++) {
        dijkstraPath(kingdomVertexNumber, i, mapWidth*mapHeight);
    }

    Texture2D mapTileSet = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\initial map (1).png");
    Texture2D GroundTile = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\tilecrop (1).png");
    Texture2D Castle = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone castle.png");
    Texture2D Stone = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone2.png");
    Texture2D House = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\house.png");

    Font font= LoadFont("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\pixantiqua.png");

    kingdoms[0].color = WHITE;
    kingdoms[1].color = (Color){59, 114, 209, 255};
    kingdoms[2].color = (Color){227, 125, 130, 255};
    kingdoms[3].color = (Color){131, 235, 148, 255};
    kingdoms[4].color = (Color){189, 125, 219, 255};

    SetTargetFPS(60);

    float buttonsPosY = -100;

//     Main game loop
    while (!WindowShouldClose()) {
        //Hover effect vectors
        mousePosition = GetMousePosition();
        coordination = (Vector2){(int) ((mousePosition.x - map0.x) / TILE_SIZE), (int) ((mousePosition.y - map0.y) / TILE_SIZE)};

        BeginDrawing();

        ClearBackground(BLACK);

        mapDrawer(mapTileSet, GroundTile, Castle, House, Stone, font, map0);

        for (int i = 0; i < kingdomNumber; ++i) {
            char text[50];
            sprintf(text, "Kingdom %d", i + 1);
            DrawText(text, SCREEN_WIDTH - 285, 15 + (SCREEN_HEIGHT / kingdomNumber) * i, 30, kingdoms[i+1].color);
            sprintf(text, "Food = %d    FoodX = %d", kingdoms[i+1].food, kingdoms[i+1].foodX);
            DrawText(text, SCREEN_WIDTH - 285, 50 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, kingdoms[i+1].color);
            sprintf(text, "Gold = %d    GoldX = %d", kingdoms[i+1].gold, kingdoms[i+1].goldX);
            DrawText(text, SCREEN_WIDTH - 285, 75 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, kingdoms[i+1].color);
            sprintf(text, "Soldiers = %d", kingdoms[i+1].soldier);
            DrawText(text, SCREEN_WIDTH - 285, 100 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, kingdoms[i+1].color);
            sprintf(text, "Workers = %d", kingdoms[i+1].worker);
            DrawText(text, SCREEN_WIDTH - 285, 125 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, kingdoms[i+1].color);
            sprintf(text, "Villages = %d", kingdoms[i+1].villageNumber);
            DrawText(text, SCREEN_WIDTH - 285, 150 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, kingdoms[i+1].color);
            DrawRectangle(SCREEN_WIDTH - 300, (SCREEN_HEIGHT / kingdomNumber) * i - 4, 300, 4, GREEN);
        }


        if (mode == 0) {

            kingdomVertexNumber = kingdoms[turn].y * mapWidth + kingdoms[turn].x;
            for (int i = 0; i < villageNumber; i++) {
                dijkstraPath(kingdomVertexNumber, i, mapWidth * mapHeight);
            }

            if (turn > kingdomNumber) {
                turn = 1;
                for (int i = 1; i <= kingdomNumber; i++) {
                    kingdoms[i].food += kingdoms[i].foodX;
                    kingdoms[i].gold += kingdoms[i].goldX;
                }
            }

            strcpy(buttons[0].text, "Add food");
            strcpy(buttons[1].text, "Add worker");
            strcpy(buttons[2].text, "Add soldier");
            strcpy(buttons[3].text, "Make road");
            strcpy(buttons[4].text, "Do nothing");
            for (int i = 0; i < 5; ++i) {
                buttons[i].rect.x = 30 + 175 * i;
                if (buttonsPosY < 20) buttonsPosY += GetFrameTime() * 30;
                buttons[i].rect.y = buttonsPosY;
                buttons[i].rect.width = 150;
                buttons[i].rect.height = 65;
                buttons[i].color = WHITE;
                if (CheckCollisionPointRec(mousePosition, buttons[i].rect)) {
                    buttons[i].color = (Color) {255, 255, 255, 220};
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        mode = 1;
                        action = i + 1;
                    }
                }
                DrawRectangle(buttons[i].rect.x, buttons[i].rect.y, buttons[i].rect.width, buttons[i].rect.height,
                              buttons[i].color);
                DrawText(buttons[i].text, buttons[i].rect.x + 25, buttons[i].rect.y + 25, 20, BLACK);
            }

        }

        if (mode == 1) {
            buttonsPosY = -100;
            switch (action) {
                case 1:
                    if (kingdoms[turn].gold >= 1) {
                        kingdoms[turn].gold--;
                        kingdoms[turn].food++;

                    } else {
                        DrawRectangle(90, 90, 710, 40, (Color) {222, 131, 124, 100});
                        DrawText("You don't have enough gold to buy food!!!", 100, 100, 30, RED);
                        EndDrawing();
                        WaitTime(2);
                        turn--;
                    }

                    break;

                case 2:
                    if (kingdoms[turn].food >= 3) {
                        kingdoms[turn].food -= 3;
                        kingdoms[turn].worker++;
                    } else {
                        DrawRectangle(90, 90, 720, 40, (Color) {222, 131, 124, 100});
                        DrawText("You don't have enough food to hire workers!!!", 100, 100, 30, RED);
                        EndDrawing();
                        WaitTime(2);
                        turn--;
                    }
                    break;

                case 3:
                    if (kingdoms[turn].gold >= 3) {
                        kingdoms[turn].gold -= 3;
                        kingdoms[turn].soldier++;
                    } else {
                        DrawRectangle(90, 90, 720, 40, (Color) {222, 131, 124, 100});
                        DrawText("You don't have enough gold to hire soldiers!!!", 100, 100, 30, RED);
                        EndDrawing();
                        WaitTime(2);
                        turn--;
                    }
                    break;

                case 4:
                    mode=2;
                    continue;
                case 5:
                    break;
            }

            int areAllVillagesConquered = 0, areSoldiersEnough = 0;
            if (kingdoms[turn].villageNumber == villageNumber) areAllVillagesConquered = 1;
            if (kingdoms[turn].soldier >= neededSoldier) areSoldiersEnough = 1;
            if (areAllVillagesConquered && areSoldiersEnough) {
                winner = turn;
                mode = 3;
            }
            else mode = 0;
            turn++;
        }

        if (mode == 2) {
            int check = checkNeighbors(kingdoms[turn].x, kingdoms[turn].y, map0);

            for (int mapX = 0; mapX < mapWidth; ++mapX) {
                for (int mapY = 0; mapY < mapHeight; ++mapY) {
                    if (kingdoms[turn].roads[mapX][mapY] == 0) check = checkNeighbors(mapX, mapY, map0);
                }
            }

            for(int i=0; i<villageNumber && check==0; i++) {
                if(villages[i].kingdom==turn) {
                    check = checkNeighbors(villages[i].x, villages[i].y, map0);
                }
            }

        }

        if (mode == 3) {
            DrawRectangle(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){128, 128, 128, 150});
            char text[20];
            sprintf(text, "WINNER IS KINGDOM %d", winner);
            DrawText(text, 200, 200, 40, BLACK);
        }

            EndDrawing();
        }

        UnloadTexture(mapTileSet);
        UnloadTexture(GroundTile);
        // De-Initialization
        CloseWindow();

        return 0;
    }
