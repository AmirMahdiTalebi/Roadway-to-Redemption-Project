#include <stdio.h>
#include "raylib.h"
#include "InitialMap.h"

// Program main entry point
int main() {

    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "starting window");

    Vector2 mapCenter= {TILE_SIZE*12, TILE_SIZE*11};
    Vector2 map0= {mapCenter.x - (mapWidth*TILE_SIZE/2), mapCenter.y - (mapHeight*TILE_SIZE/2)};

    Texture2D mapTileSet = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\initial map (1).png");
    Texture2D GroundTile = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\tilecrop (1).png");
    Texture2D Castle = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\castle.png");
    Texture2D Stone = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\stone2.png");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        //Hover effect vectors
        Vector2 mousePosition = GetMousePosition();
        Vector2 coordination = {(int)((mousePosition.x - map0.x)/TILE_SIZE), (int)((mousePosition.y - map0.y)/TILE_SIZE)};

        mapDrawer(mapTileSet, GroundTile, Castle, Stone, map0, coordination);
    }

    UnloadTexture(mapTileSet);
    UnloadTexture(GroundTile);
    // De-Initialization
    CloseWindow();

    return 0;
}
