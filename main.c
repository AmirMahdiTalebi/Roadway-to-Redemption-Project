#include <stdio.h>
#include "raylib.h"
#include "InitialMap.h"
#include <limits.h>

int list[MAP_SIZE*MAP_SIZE][5];

//int dijkstra(int source, int dest) {
//    //vars
//    int dist[5]={INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};
//    bool visited[5]= {0,0,0,0,0};
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
//        visited[current]=true;
//
//        //update current
//        minDistance=INT_MAX;
//        for(int i=0; i<5; i++) {
//            if(!visited[i] && dist[i]<minDistance) {
//                current=i;
//                minDistance=dist[i];
//            }
//        }
//    }
//
//    //output
//    for(int k=0; k<5; k++) {
//        printf("%d %d\n", k, dist[k]);
//    }
//    return 0;
//}

//int dijkstraPath(int source, int dest) {
//    //vars
//    int dist[5]={INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};
//    int path[5][5]={
//            {source,-1,-1,-1,-1},
//            {source,-1,-1,-1,-1},
//            {source,-1,-1,-1,-1},
//            {source,-1,-1,-1,-1},
//            {source,-1,-1,-1,-1}
//    };
//    int pathNumber[5]={0,0,0,0,0,};
//    bool visited[5]= {0,0,0,0,0};
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
//            if(!visited[neighbor] && (dist[current] + list[neighbor][0]) < dist[neighbor]) {
//                //update distance
//                dist[neighbor] = dist[current] + list[neighbor][0];
//                pathNumber[neighbor]= pathNumber[current]+1;
//                //update path
//                for(int j=0; j<pathNumber[current]; j++) {
//                    path[neighbor][j]=path[current][j];
//                }
//                path[neighbor][pathNumber[neighbor]-1]=neighbor;
//            }
//        }
//
//        //current=visited
//        visited[current]=true;
//
//        //update current
//        minDistance=INT_MAX;
//        for(int i=0; i<5; i++) {
//            if(!visited[i] && dist[i]<minDistance) {
//                current=i;
//                minDistance=dist[i];
//            }
//        }
//    }
//
//    //output
//    for(int k=0; k<5; k++) {
//        printf("shortest path from %d to %d is ", source, k);
//        for(int l=0; l<pathNumber[k]; l++)
//            printf("%d", path[k][l]);
//        printf("\n");
//    }
//    return 0;
//}

//int main(void)
//{
//    int source, dest;
//    scanf("%d %d", &source, &dest);
//    //dijkstra(source, dest);
//    dijkstraPath(source,dest);
//    return 0;
//}

// Program main entry point
int main() {

    initialMapMaker();
    makeKingdom();
    makeVillage();
    makeBarrier();

    //Creating dijkstra list
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            if (map[0][j][i] < 0) {
                list[i*mapWidth+j][0] = INT_MAX;
            }
            else list[i*mapWidth+j][0] = map[0][j][i];
        }
    }

    //Adding neighbors to the list
    for (int i = 0; i < mapWidth*mapHeight; ++i) {
        int k = 0;
        int e = 0;
        int verticeIndex = i + 1;
        if ((verticeIndex - mapWidth) <= 0) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - mapWidth;
            k++;
        }
        if ((verticeIndex + mapWidth) > (mapWidth * mapHeight)) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i + mapWidth;
            k++;
        }
        if ((verticeIndex - 1) % mapWidth == 0) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - 1;
            k++;
        }
        if ((verticeIndex) % mapWidth == 0) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i + 1;
            k++;
        }
    }

        for (int j = 0; j < mapHeight*mapWidth; ++j) {
            for (int k = 0; k < 5; ++k) {
                printf("%d ", list[j][k]);
            }
            printf("\n");
        }


//    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "starting window");
//
//    Vector2 mapCenter= {TILE_SIZE*12, TILE_SIZE*11};
//    Vector2 map0= {mapCenter.x - (mapWidth*TILE_SIZE/2), mapCenter.y - (mapHeight*TILE_SIZE/2)};
//
//    Texture2D mapTileSet = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\initial map (1).png");
//    Texture2D GroundTile = LoadTexture("D:\\roadway\\Roadway-to-Redemption-Project\\assets\\FieldsTile_01.png");
//
//    SetTargetFPS(60);
//
//    // Main game loop
//    while (!WindowShouldClose())
//    {
//        //Hover effect vectors
//        Vector2 mousePosition = GetMousePosition();
//        Vector2 coordination = {(int)((mousePosition.x - map0.x)/TILE_SIZE), (int)((mousePosition.y - map0.y)/TILE_SIZE)};
//
//        mapDrawer(mapTileSet, map0, coordination);
//    }
//
//    UnloadTexture(mapTileSet);
//    // De-Initialization
//    CloseWindow();

    return 0;
}
