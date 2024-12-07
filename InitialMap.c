#include <stdio.h>
#include "raylib.h"
#include "InitialMap.h"

// Global Variables
int map[4][MAP_SIZE][MAP_SIZE];
int mapHeight, mapWidth;

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
            map[0][j][i] = 1;
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

int mapDrawer() {
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            switch (map[0][j][i]) {
                case -1: // -1 is the code for kingdoms.
                    printf("C ");
                    break;
                case -2: // -2 is the code for villages.
                    printf("V ");
                    break;
                case -3: // 0 is the code for barriers.
                    printf("X ");
                    break;
                default: // It is used for roadways' numbers.
                    printf("%d ", map[0][j][i]);
            }
        }
        printf("\n");
    }
    return 0;
}