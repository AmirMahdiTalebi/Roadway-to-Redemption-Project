#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <conio.h>
#include <time.h>
#include "InitialMap.h"

typedef struct kingdom kingdom;
typedef struct village village;

//make village id and kingdom id more accessible and call dijkstra after getting all the x and y
//maybe make a function that stores all the paths like intial mapmakers

// Global Variables
int map[4][MAP_SIZE][MAP_SIZE];
int mapHeight = 0;
int mapWidth = 0;
int list[MAP_SIZE*MAP_SIZE][5];
int turn=0;


village villages[30]={0};


kingdom kingdoms[5]={0};


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

int dijkstraPath(int source, int dest, village village1, int size) {

    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            if (map[0][i][j] < 0) {
                list[j*mapWidth+i][0] = -1;
            }
            else list[j*mapWidth+i][0] = map[0][i][j];
        }
    }
    list[source][0]=list[dest][0]=0;

    //Adding neighbors to the list
    for (int i = 0; i < mapWidth*mapHeight; ++i) {
        int k = 0;
        int e = 0;
        int verticeIndex = i + 1;
        if ((verticeIndex - mapWidth) <= 0 || list[i - mapWidth][0] == -1) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - mapWidth;
            k++;
        }
        if ((verticeIndex + mapWidth) > (mapWidth * mapHeight) || list[i + mapWidth][0] == -1) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i + mapWidth;
            k++;
        }
        if (verticeIndex % mapWidth == 1 || list[i-1][0] == -1) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - 1;
            k++;
        }
        if ((verticeIndex) % mapWidth == 0 || list[i+1][0] == -1) list[i][4-e] = -1;
        else list[i][1+k] = i + 1;
    }

    //vars
    int dist[size];
    for (int i = 0; i < size; ++i) {
        dist[i] = INT_MAX;
    }
    int path[size][size];
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j == 0) path[i][j] = source;
            else path[i][j] = -1;
        }
    }
    int pathNumber[289]= {0};
    int visited[289]= {0};
    dist[source]=0;
    pathNumber[source]=1;
    int current=source, neighbor, minDistance;

    //main algorithm loop
    while(!visited[dest]) {

        //update all neighbors
        for(int i=1; list[current][i]>=0; i++) {
            neighbor=list[current][i];
            if(!visited[neighbor] && (dist[current] + list[neighbor][0]) < dist[neighbor] && list[neighbor][0] != -1) {
                //update distance
                dist[neighbor] = dist[current] + list[neighbor][0];
                //update path
                pathNumber[neighbor]= pathNumber[current]+1;
                for(int j=0; j<pathNumber[current]; j++) {
                    path[neighbor][j]=path[current][j];
                }
                path[neighbor][pathNumber[neighbor]-1]=neighbor;
            }
        }

        //current is visited
        visited[current] = 1;

        //update current
        minDistance=INT_MAX;
        for(int i=0; i<size; i++) {
            if(!visited[i] && dist[i]<minDistance && list[i][0] != -1) {
                current=i;
                minDistance=dist[i];
            }
        }
    }

    printf("shortest path (%d) from %d to %d(%d) is " ,dist[dest],source, dest, list[dest][0]);
    for(int l=0; l<pathNumber[dest]; l++)
        printf("%d ", path[dest][l]);
    printf("\n");

//    int id=map[1][x][y];
//    int x=dest%mapWidth, y=dest/mapWidth, k;
//    for(k=0; k<pathNumber[dest]; k++) {
//        villagePath[map[1][x][y]][k]=path[dest][k];
//    }
//    villa[map[1][x][y]][k]=-1;
    return 0;
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
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            map[0][i][j] = generate_number();
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
        kingdoms[i].x = x;
        kingdoms[i].y = y;
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
        villages[i].x = x;
        villages[i].y = y;

        printf("goldX:");
        scanf("%d", &goldX);
        while(goldX<0) {
            printf("Number is invalid.\ngoldX:");
            scanf("%d", &goldX);
        }
        map[2][x][y] = goldX;
        villages[i].goldX = goldX;

        printf("foodX:");
        scanf("%d", &foodX);
        while(foodX<0) {
            printf("Number is invalid.\nfoodX:");
            scanf("%d", &foodX);
        }
        map[3][x][y] = foodX;
        villages[i].foodX = foodX;
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
    for(int i=0; i<mapWidth; i++) {
        for(int j=0; j<mapHeight; j++) {
            if(coordination.x==i && coordination.y==j && map[0][i][j]==-2) {
                int id= map[1][i][j];
                for(int k=0; villages[id].pathNumber; k++) {
                    DrawRectangle((villages[id].path[k] % mapWidth) * TILE_SIZE + map0.x, (villages[id].path[k] / mapWidth) * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, (Color){255, 255, 255, 100});
                }
            }
        }
    }

    EndDrawing();
    return 0;
}