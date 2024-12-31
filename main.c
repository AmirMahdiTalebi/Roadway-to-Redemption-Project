#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "InitialMap.h"

// Program main entry point
int main() {
    action = 0;
    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "starting window");

    Vector2 mapCenter= {TILE_SIZE*12, TILE_SIZE*11};
    Vector2 map0= {mapCenter.x - (mapWidth*TILE_SIZE/2), mapCenter.y - (mapHeight*TILE_SIZE/2)};

    buttonsPosY = -100;

    Texture2D mapTileSet = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\initial map (1).png");
    Texture2D GroundTile = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\tilecrop (1).png");
    Texture2D Castle = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone castle.png");
    Texture2D Stone = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone2.png");
    Texture2D House = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\house.png");
    Texture2D roadMan = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\SteamMan_attack1.png");
    Texture2D explosion = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\explosion.png");

    Font font= LoadFont("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\pixantiqua.png");

    kingdoms[0].color = WHITE;
    kingdoms[1].color = (Color){103, 135, 194, 255};
    kingdoms[2].color = (Color){230, 37, 82, 255};
    kingdoms[3].color = (Color){131, 235, 148, 255};
    kingdoms[4].color = (Color){189, 125, 219, 255};

    SetTargetFPS(60);
    int manIndex = 0;
    float manTimer =MAN_TIME;
    float flameTimer = FLAME_TIME;
    int AnimationCounter = 0;

    //Main game loop
    while (!WindowShouldClose()) {
        //Hover effect vectors
        mousePosition = GetMousePosition();
        coordination = (Vector2){(int) ((mousePosition.x - map0.x) / TILE_SIZE), (int) ((mousePosition.y - map0.y) / TILE_SIZE)};

        BeginDrawing();

        ClearBackground(BLACK);

        mapDrawer(mapTileSet, GroundTile, Castle, House, Stone, font, map0);

        for (int i = 0; i < kingdomNumber; ++i) { //show kingdoms' info
            char text[50];
            Color textColor;
            if(kingdoms[i+1].dead)
                textColor=(Color){30,30,30,255};
            else if(i+1!=turn)
                textColor=transparentWhite;
            else
                textColor=kingdoms[i+1].color;
            sprintf(text, "Kingdom %d", i + 1);
            DrawText(text, SCREEN_WIDTH - 285, 15 + (SCREEN_HEIGHT / kingdomNumber) * i, 30, textColor);
            sprintf(text, "Food = %d    FoodX = %d", kingdoms[i+1].food, kingdoms[i+1].foodX);
            DrawText(text, SCREEN_WIDTH - 285, 50 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Gold = %d    GoldX = %d", kingdoms[i+1].gold, kingdoms[i+1].goldX);
            DrawText(text, SCREEN_WIDTH - 285, 75 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Soldiers = %d", kingdoms[i+1].soldier);
            DrawText(text, SCREEN_WIDTH - 285, 100 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Workers = %d", kingdoms[i+1].worker);
            DrawText(text, SCREEN_WIDTH - 285, 125 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            sprintf(text, "Villages = %d", kingdoms[i+1].villageNumber);
            DrawText(text, SCREEN_WIDTH - 285, 150 + (SCREEN_HEIGHT / kingdomNumber) * i, 20, textColor);
            DrawRectangle(SCREEN_WIDTH - 300, (SCREEN_HEIGHT / kingdomNumber) * i - 4, 300, 4, GREEN);
        }


        if (mode == 0) //start of the turn
            mode0();

        if (mode == 1)  //actions
            mode1();

        if (mode == 2) { //making roads
            int check = checkNeighbors(kingdoms[turn].x, kingdoms[turn].y, map0);

            for (int i = 0; i <kingdoms[turn].roadNumber && check==0; ++i) {
                check = checkNeighbors(kingdoms[turn].road[i].x, kingdoms[turn].road[i].y, map0);
            }

            for(int i=0; i<villageNumber && check==0; i++) {
                if(villages[i].kingdom==turn) {
                    check = checkNeighbors(villages[i].x, villages[i].y, map0);
                }
            }

        }

        if (mode == 3) { //the end of the game
            DrawRectangle(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){128, 128, 128, 150});
            char text[20];
            sprintf(text, "WINNER IS KINGDOM %d", winner);
            DrawText(text, 200, 200, 40, BLACK);
        }

        if (mode==4) { //animation
            if (animation == 1) {
                if(AnimationCounter < 12) {
                    manTimer -= GetFrameTime();
                    if(manTimer < 0) {
                        AnimationCounter++;
                        manTimer = MAN_TIME;
                        manIndex++;
                        if (manIndex >= MAN_COUNT) manIndex = 0;
                    }
                    Rectangle source = (Rectangle){TILE_SIZE * manIndex,0,TILE_SIZE,TILE_SIZE};
                    DrawTextureRec(roadMan, source, manPos, WHITE);
                }
                else {
                    RoadMaker();
                    AnimationCounter = 0;
                }
            }
            else if (animation == 2) {
                if(AnimationCounter < 28) {
                    flameTimer -= GetFrameTime();
                    if(flameTimer < 0) {
                        AnimationCounter++;
                        flameTimer = FLAME_TIME;
                        manIndex++;
                        if (manIndex >= FLAME_COUNT) manIndex = 0;
                    }
                    manPos = (Vector2) {(kingdoms[toBeDeleted].x-.3)*TILE_SIZE+map0.x, (kingdoms[toBeDeleted].y-.4)*TILE_SIZE+map0.y};
                    Rectangle source = (Rectangle){FLAME_WIDTH * manIndex,0,FLAME_WIDTH,FLAME_HEIGHT};
                    DrawTextureRec(explosion, source, manPos, WHITE);
                }
                else {
                    AnimationCounter = 0;
                    DeleteKingdom(toBeDeleted);
                }
            }
        }

        EndDrawing();
    }



    UnloadTexture(mapTileSet);
    UnloadTexture(GroundTile);
    UnloadTexture(Stone);
    UnloadTexture(House);
    UnloadTexture(Castle);
    UnloadTexture(roadMan);
    UnloadTexture(explosion);
    UnloadFont(font);
    // De-Initialization
    CloseWindow();

    return 0;
}
