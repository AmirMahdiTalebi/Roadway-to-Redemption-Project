#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <conio.h>
#include <time.h>
#include "InitialMap.h"
#include <limits.h>


//int dijkstra(int source, int dest) {
//    //vars
//    int dist[289];
//    for (int i = 0; i < 289; ++i) {
//        dist[i] = INT_MAX;
//    }
//    int visited[289] = {0};
//    dist[source]=0;
//    int current=source, neighbor, minDistance;
//
//    //main algorithm loop
//    while(!visited[dest]) {
//
//        //update all neighbors
//        for(int i=1; list[current][i]>=0; i++) {
//            neighbor=list[current][i];
//            if(!visited[neighbor] && (dist[current] + list[neighbor][0]) < dist[neighbor])
//                dist[neighbor]= dist[current] + list[neighbor][0];
//        }
//
//        //current=visited
//        visited[current]= 1;
//
//        //update current
//        minDistance=INT_MAX;
//        for(int i=0; i<289; i++) {
//            if(!visited[i] && dist[i]<minDistance) {
//                current=i;
//                minDistance=dist[i];
//            }
//        }
//    }
//
//    //output
//    for(int k=0; k< mapWidth*mapHeight; k++) {
//        printf("%d %d\n", k, dist[k]);
//    }
//    return 0;
//}

//int dijkstraPath(int source, int dest, int size) {
//    //vars
//    int dist[size];
//    for (int i = 0; i < size; ++i) {
//        dist[i] = INT_MAX;
//    }
//    int path[size][size];
//    for (int i = 0; i < size; ++i) {
//        for (int j = 0; j < size; ++j) {
//            if (j == 0) path[i][j] = source;
//            else path[i][j] = -1;
//        }
//    }
//    int pathNumber[289]= {0};
//    int visited[289]= {0};
//    dist[source]=0;
//    pathNumber[source]=1;
//    int current=source, neighbor, minDistance;
//
//    //main algorithm loop
//    while(!visited[dest]) {
//
//        //update all neighbors
//        for(int i=1; list[current][i]>=0; i++) {
//            neighbor=list[current][i];
//            if(!visited[neighbor] && (dist[current] + list[neighbor][0]) < dist[neighbor] && list[neighbor][0] != -1) {
//                //update distance
//                dist[neighbor] = dist[current] + list[neighbor][0];
//                //update path
//                pathNumber[neighbor]= pathNumber[current]+1;
//                for(int j=0; j<pathNumber[current]; j++) {
//                    path[neighbor][j]=path[current][j];
//                }
//                path[neighbor][pathNumber[neighbor]-1]=neighbor;
//            }
//        }
//
//        //current is visited
//        visited[current] = 1;
//
//        //update current
//        minDistance=INT_MAX;
//        for(int i=0; i<size; i++) {
//            if(!visited[i] && dist[i]<minDistance && list[i][0] != -1) {
//                current=i;
//                minDistance=dist[i];
//            }
//        }
//    }

    //output
//    for(int k=0; k < mapWidth*mapHeight; k++) {
//        printf("shortest path (%d) from %d to %d(%d) is " ,dist[k],source, k, list[k][0]);
//        for(int l=0; l<pathNumber[k]; l++)
//            printf("%d ", path[k][l]);
//        printf("\n");
//    }
//    return 0;
//}

// Program main entry point
int main() {

    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();

    //Creating dijkstra list
//    for (int i = 0; i < mapWidth; ++i) {
//        for (int j = 0; j < mapHeight; ++j) {
//            if (map[0][i][j] < 0) {
//                list[j*mapWidth+i][0] = -1;
//            }
//            else list[j*mapWidth+i][0] = map[0][i][j];
//        }
//    }
//
//    //Adding neighbors to the list
//    for (int i = 0; i < mapWidth*mapHeight; ++i) {
//        int k = 0;
//        int e = 0;
//        int verticeIndex = i + 1;
//        if ((verticeIndex - mapWidth) <= 0 || list[i - mapWidth][0] == -1) {
//            list[i][4-e] = -1;
//            e++;
//        }
//        else {
//            list[i][1+k] = i - mapWidth;
//            k++;
//        }
//        if ((verticeIndex + mapWidth) > (mapWidth * mapHeight) || list[i + mapWidth][0] == -1) {
//            list[i][4-e] = -1;
//            e++;
//        }
//        else {
//            list[i][1+k] = i + mapWidth;
//            k++;
//        }
//        if (verticeIndex % mapWidth == 1 || list[i-1][0] == -1) {
//            list[i][4-e] = -1;
//            e++;
//        }
//        else {
//            list[i][1+k] = i - 1;
//            k++;
//        }
//        if ((verticeIndex) % mapWidth == 0 || list[i+1][0] == -1) list[i][4-e] = -1;
//        else list[i][1+k] = i + 1;
//    }
//    for(int i=0; i<mapWidth*mapHeight; i++) {
//                printf("%d ", i);
//                for(int k=0; k<5; k++) {
//                    printf("%d ", list[i][k]);
//                }
//                printf("\n");
//        }


//    int source1, dest1;
//    source1 = 0;
//    dest1 = 90;
//    //dijkstra(source1, dest1);
//    dijkstraPath(source1,dest1, mapWidth*mapHeight);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "starting window");

    Vector2 mapCenter= {TILE_SIZE*15, TILE_SIZE*13};
    Vector2 map0= {mapCenter.x - (mapWidth*TILE_SIZE/2), mapCenter.y - (mapHeight*TILE_SIZE/2)};
//
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