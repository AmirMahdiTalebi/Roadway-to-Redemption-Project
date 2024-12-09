#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <conio.h>
#include <time.h>
#include "InitialMap.h"

// Global Variables
int map[4][MAP_SIZE][MAP_SIZE];
int mapHeight, mapWidth;

int generate_number() {
    float probs[4] = {.65, .25, .05, .05};

    float randValue = GetRandomValue(0, 100) / 100.0f;

    if (randValue < probs[0]) {
        return 1;
    } else if (randValue < probs[0] + probs[1]) {
        return 2;
    } else if (randValue < probs[0] + probs[1] + probs[2]) {
        return 3;
    } else {
        return 4;
    }
}

int initialMapMaker() {
    // Getting map's width and height
    printf("Enter the map height:");
    scanf("%d", &mapHeight);
    while(mapHeight<3 || mapHeight>MAP_SIZE) {
        printf("Map height is invalid.\nEnter the map height:");
        scanf("%d", &mapHeight);
    }
    printf("Enter the map width:");
    scanf("%d", &mapWidth);
    while(mapWidth<3 || mapWidth>MAP_SIZE) {
        printf("Map width is invalid.\nEnter the map width:");
        scanf("%d", &mapWidth);
    }
    // Initializing map's values
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            map[0][j][i] = generate_number();
        }
    }
    return 0;
}

int makeKingdom() {
    int n, x, y;
    printf("Enter the number of kingdoms:");
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        printf("Enter x and y for the kingdom No.%d:\n", i + 1);
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("x and y are invalid.\nEnter x and y for the kingdom No.%d:", i + 1);
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nEnter x and y for the kingdom No.%d:", i + 1);
            scanf("%d %d", &x, &y);
        }
        //x and y start from 1
        x--;
        y--;
        map[0][x][y] = -1; // -1 is the code for kingdoms.
        map[1][x][y] = i; // I saved Kingdoms' IDs in z=1.
    }
    return 0;
}

int makeVillage() {
    int n, x, y, goldX, foodX;
    printf("Enter the number of villages:");
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        printf("Enter the info for the village No.%d:\n", i + 1);
        printf("x and y:");
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("X and y are invalid.\nx and y:");
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nx and y:");
            scanf("%d %d", &x, &y);
        }
        // x and y start from 1
        x--;
        y--;
        map[0][x][y] = -2; // -2 is the code for villages.
        map[1][x][y] = i; // I saved villages' IDs in z=1.
        printf("goldX:");
        scanf("%d", &goldX);
        while(goldX<0) {
            printf("Number is invalid.\ngoldX:");
            scanf("%d", &goldX);
        }
        map[2][x][y] = goldX;
        printf("foodX:");
        scanf("%d", &foodX);
        while(foodX<0) {
            printf("Number is invalid.\nfoodX:");
            scanf("%d", &foodX);
        }
        map[3][x][y] = foodX;
    }
    return 0;
}

int makeBarrier() {
    int n, x, y;
    printf("Enter the number of barriers:");
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        printf("Enter the x and y of the barrier:");
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("X and y are invalid.\nEnter the x and y of the barrier:");
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nEnter the x and y of the barrier:");
            scanf("%d %d", &x, &y);
        }
        //x and y start from 1
        x--;
        y--;
        map[0][x][y] = -3; // -3 is the code for barriers.
    }
    return 0;
}

int mapDrawer(Texture2D mapTileSet, Vector2 map0, Vector2 coordination) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawTexture(mapTileSet, 0,0, WHITE);

    for(int i=0; i<mapWidth; i++) {
        for(int j=0; j<mapHeight; j++) {

            //MapDrawer
            switch (map[0][i][j]) {
                case -1: // -1 is the code for kingdoms.
                    DrawRectangle(i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE, BLACK);
                    break;
                case -2: // -2 is the code for villages.
                    DrawRectangle(i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE, BROWN);
                    break;
                case -3: // -3 is the code for barriers.
                    DrawRectangle(i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE, GRAY);
                    break;
                default: // It is used for roadways' numbers.
                    DrawRectangleLines(i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE, BROWN);

                    // Writing roadways' numbers on the tile
                    char innerNum[2];
                    sprintf(innerNum, "%d", map[0][i][j]);
                    DrawText(innerNum, (i+.5)*TILE_SIZE+map0.x, (j+.5)*TILE_SIZE+map0.y, 20, BLACK);
            }

            // Hover Effect
            if(coordination.x==i && coordination.y==j) {
                DrawRectangle(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, (Color){255, 255, 255, 100});
            }
        }
    }

    EndDrawing();
    return 0;
}