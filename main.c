#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <conio.h>
#include <time.h>
#include "InitialMap.h"
#include <limits.h>

// Program main entry point
int main() {

    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();

    Vector2 mapCenter= {TILE_SIZE*15, TILE_SIZE*13};
    Vector2 map0= {mapCenter.x - (mapWidth*TILE_SIZE/2), mapCenter.y - (mapHeight*TILE_SIZE/2)};
    int kingdomVerticeNumber = kingdoms[turn].y*mapWidth + kingdoms[turn].x;
    dijkstraPath(kingdomVerticeNumber, 0, mapWidth*mapHeight);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "starting window");


    Texture2D mapTileSet = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\Initial_Map.png");
//
    SetTargetFPS(60);

//     Main game loop
    while (!WindowShouldClose()) {
//        Hover effect vectors
        Vector2 mousePosition = GetMousePosition();
        Vector2 coordination = {(int)((mousePosition.x - map0.x)/TILE_SIZE), (int)((mousePosition.y - map0.y)/TILE_SIZE)};

        mapDrawer(mapTileSet, map0, coordination);
    }

    UnloadTexture(mapTileSet);
    // De-Initialization
    CloseWindow();

    return 0;
}