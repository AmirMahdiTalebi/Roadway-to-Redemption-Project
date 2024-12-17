#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "InitialMap.h"

int mode = 0;
struct button {
    Rectangle rect;
    char text[30];
    Color color;
};
typedef struct button button;
button buttons[5];


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
    int kingdomVerticeNumber = kingdoms[turn].y*mapWidth + kingdoms[turn].x;
    for(int i=0; i<villageNumber; i++) {
        dijkstraPath(kingdomVerticeNumber, i, mapWidth*mapHeight);
    }

    Texture2D mapTileSet = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\initial map (1).png");
    Texture2D GroundTile = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\tilecrop (1).png");
    Texture2D Castle = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone castle.png");
    Texture2D Stone = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone2.png");
    Texture2D House = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\house.png");

    Font font= LoadFont("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\pixantiqua.png");

    SetTargetFPS(60);

    float buttonsPosY = -100;

//     Main game loop
    while (!WindowShouldClose()) {
//        Hover effect vectors
        Vector2 mousePosition = GetMousePosition();
        Vector2 coordination = {(int)((mousePosition.x - map0.x)/TILE_SIZE), (int)((mousePosition.y - map0.y)/TILE_SIZE)};

        BeginDrawing();

        ClearBackground(RAYWHITE);

        mapDrawer(mapTileSet, GroundTile, Castle, House, Stone, font, map0, coordination);
        scoreBoardDrawer(font);

        if (mode == 0) {
            strcpy(buttons[0].text, "Add food");
            strcpy(buttons[1].text, "Add worker");
            strcpy(buttons[2].text, "Add soldier");
            strcpy(buttons[3].text, "Make road");
            strcpy(buttons[4].text, "Do nothing");
            for (int i = 0; i < 5; ++i) {
                buttons[i].rect.x = 30 + 175*i;
                if (buttonsPosY < 20) buttonsPosY += GetFrameTime() * 30;
                buttons[i].rect.y = buttonsPosY;
                buttons[i].rect.width = 150;
                buttons[i].rect.height = 65;
                buttons[i].color = WHITE;
                if (CheckCollisionPointRec(mousePosition, buttons[i].rect))
                {
                    buttons[i].color = (Color){255, 255, 255, 220};
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        mode = 1;
                        action = i + 1;
                    }
                }
                DrawRectangle(buttons[i].rect.x, buttons[i].rect.y, buttons[i].rect.width, buttons[i].rect.height, buttons[i].color);
                DrawText(buttons[i].text, buttons[i].rect.x + 25, buttons[i].rect.y + 25, 20, BLACK);
            }

        }

        if (mode == 1) {
            buttonsPosY = -100;
            switch (action) {
                case 0:
                    return 0;

                case 1:
                    if (kingdoms[turn].gold >= 1) {
                        kingdoms[turn].gold--;
                        kingdoms[turn].food++;

                        kingdoms[turn].food+=kingdoms[turn].foodX;
                        kingdoms[turn].gold+=kingdoms[turn].goldX;
                    }
                    else {
                        DrawRectangle(90, 90, 700, 40, (Color){222, 131, 124, 100});
                        DrawText("You don't have enough gold to buy food!!!", 100, 100, 30, RED);
                        EndDrawing();
                        WaitTime(2);
                        turn--;
                    }

                    break;

                case 2:
                    if (kingdoms[turn].food >= 3) {
                        kingdoms[turn].food-=3;
                        kingdoms[turn].worker++;

                        kingdoms[turn].food+=kingdoms[turn].foodX;
                        kingdoms[turn].gold+=kingdoms[turn].goldX;
                        turn--;
                    }
                    else {
                        DrawRectangle(90, 90, 700, 40, (Color){222, 131, 124, 100});
                        DrawText("You don't have enough food to hire workers!!!", 100,100, 30, RED);
                        EndDrawing();
                        WaitTime(2);
                        turn--;
                    }
                    break;

                case 3:
                    if (kingdoms[turn].gold >= 2) {
                        kingdoms[turn].gold-=3;
                        kingdoms[turn].soldier++;

                        kingdoms[turn].food+=kingdoms[turn].foodX;
                        kingdoms[turn].gold+=kingdoms[turn].goldX;
                    }
                    else {
                        DrawRectangle(90, 90, 700, 40, (Color){222, 131, 124, 100});
                        DrawText("You don't have enough gold to hire soldiers!!!", 100,100, 30, RED);
                        EndDrawing();
                        WaitTime(2);
                        turn--;
                    }
                    break;

                case 4:
//                    int sw=0;
//                    Rectangle available;
//                    while(sw==0) {
//                        BeginDrawing();
//                        for(int k=0; k<villageNumber; k++) {
//                            if(villages[k].kingdom==turn) {
//                                for(int p=1; list[villages[k].y*mapWidth+villages[k].x][p]; p++) {
//                                    Vector2 neighbor= {list[villages[k].y*mapWidth+villages[k].x][p]%mapWidth,
//                                                       list[villages[k].y*mapWidth+villages[k].x][p]/mapWidth};
//                                    DrawRectangle(neighbor.x*TILE_SIZE+map0.x, neighbor.y*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE,
//                                                  (Color){83, 212, 80, 100});
//                                    available=(Rectangle){neighbor.x*TILE_SIZE+map0.x, neighbor.y*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE};
//                                    if(CheckCollisionPointRec(mousePosition, available) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//                                        sw = 1;
//                                    }
//                                }
//                            }
//                        }
//                        EndDrawing();
//                    }

                    break;


                case 5:

                    kingdoms[turn].food+=kingdoms[turn].foodX;
                    kingdoms[turn].gold+=kingdoms[turn].goldX;

                    break;
            }
            mode = 0;
            turn++;
            if(turn>=kingdomNumber) turn=0;
            kingdomVerticeNumber = kingdoms[turn].y*mapWidth + kingdoms[turn].x;
            for(int i=0; i<villageNumber; i++) {
                dijkstraPath(kingdomVerticeNumber, i, mapWidth*mapHeight);
            }
        }

        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            mode = 0;
        }
    }

    UnloadTexture(mapTileSet);
    UnloadTexture(GroundTile);
    // De-Initialization
    CloseWindow();

    return 0;
}
