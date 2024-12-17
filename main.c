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

    Vector2 mapCenter= {TILE_SIZE*12, TILE_SIZE*12};
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

        mapDrawer(mapTileSet, GroundTile, Castle, House, Stone, font, map0, coordination);

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
                    kingdoms[turn].food++;
                    break;
                case 2:
                    if (kingdoms[turn].food > 0) {
                        kingdoms[turn].food--;
                        kingdoms[turn].worker++;
                    }
                    else {
                        DrawText("You don't have enough food to hire worker!!!", 200, 95, 20, RED);
                        turn--;
                    }
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
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
