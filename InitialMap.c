#include <stdio.h>
#include "raylib.h"
#include "InitialMap.h"

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
int map[2][MAP_SIZE][MAP_SIZE];
int mapHeight = 0;
int mapWidth = 0;
int turn = 1, kingdomNumber, villageNumber, neededSoldier;
int list[MAP_SIZE*MAP_SIZE][5];
int mode = 0;

Vector2 mousePosition;
Vector2 coordination;

Color transparentWhite = (Color){255, 255, 255, 60};
Color transparentGreen = (Color){37, 204, 81, 90};

village villages[30] = {0};
kingdom kingdoms[5] = {0};

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

int dijkstraPath(int source,int id, int size) {
    int dest = villages[id].y*mapWidth + villages[id].x;
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            if (map[0][i][j] < 0) {
                list[j*mapWidth+i][0] = 2000;
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
        if (verticeIndex % mapWidth == 1) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - 1;
            k++;
        }
        if ((verticeIndex) % mapWidth == 0) list[i][4-e] = -1;
        else list[i][1+k] = i + 1;
    }

    //vars
    int dist[size];
    for (int i = 0; i < size; ++i) {
        dist[i] = 2000;
    }
    int path[size][size];
    for (int i = 0; i < size; ++i) {
        path[i][0] = source;
    }
    int pathNumber[289]= {0};
    int visited[289]= {0};
    dist[source]=0;
    pathNumber[source]=1;
    int current=source, neighbor, minDistance;

    //main algorithm loop
    while(!visited[dest]) {

        //update all neighbors
        for(int i=1; list[current][i]>=0 && i<5; i++) {
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
        minDistance=2000;
        for(int i=0; i<size; i++) {
            if(!visited[i] && dist[i]<minDistance && list[i][0] != -1) {
                current=i;
                minDistance=dist[i];
            }
        }
    }

    int k;
    for(k=0; k<pathNumber[dest]; k++) {
        villages[id].path[k]=path[dest][k];
    }
    villages[id].pathNumber = pathNumber[dest];
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

    printf("Enter the number of soldiers that player needs to win:");
    scanf("%d", &neededSoldier);
    while(neededSoldier<0) {
        printf("Number is invalid.\nEnter the number of soldiers that player needs to win:");
        scanf("%d", &neededSoldier);
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
    int x, y;
    printf("Enter the number of kingdoms:");
    scanf("%d", &kingdomNumber);
    for (int i = 0; i < kingdomNumber; ++i) {
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
        kingdoms[i+1].x = x;
        kingdoms[i+1].y = y;
        map[0][x][y] = -1; // -1 is the code for kingdoms.
        map[1][x][y] = i+1; // I saved Kingdoms' IDs in z=1.

        kingdoms[i+1].gold = 5;
        kingdoms[i+1].food = kingdoms[i+1].foodX = kingdoms[i+1].soldier =  0;
        kingdoms[i+1].goldX = kingdoms[i+1].worker = 1;
    }
    return 0;
}

int makeVillage() {
    int x, y;
    printf("Enter the number of villages:");
    scanf("%d", &villageNumber);
    for (int i = 0; i < villageNumber; ++i) {
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
        scanf("%d", &villages[i].goldX);
        while(villages[i].goldX<0) {
            printf("Number is invalid.\ngoldX:");
            scanf("%d", &villages[i].goldX);
        }

        printf("foodX:");
        scanf("%d", &villages[i].foodX);
        while(villages[i].foodX<0) {
            printf("Number is invalid.\nfoodX:");
            scanf("%d", &villages[i].foodX);
        }
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

int mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House,Texture2D Stone, Font font, Vector2 map0) {
    DrawTexture(mapTileSet, 0,0, WHITE);
    for(int i=0; i<mapWidth; i++) {
        for(int j=0; j<mapHeight; j++) {
            //MapDrawer
            DrawTexture(GroundTile,i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, WHITE);
            DrawRectangleLines(i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE, (Color){150,75,0,100});
            switch (map[0][i][j]) {

                case -1: // -1 is the code for kingdoms.
                    DrawTexture(Castle,i*TILE_SIZE+map0.x, (j-0.5)*TILE_SIZE+map0.y, kingdoms[map[1][i][j]].color);
                    break;

                case -2: // -2 is the code for villages.
                    DrawTexture(House,i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, kingdoms[villages[map[1][i][j]].kingdom].color);
                    break;

                case -3: // -3 is the code for barriers.
                    DrawTexture(Stone,i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, WHITE);
                    break;

                default: // It is used for roadways' numbers.
                    DrawRectangleLines(i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y, TILE_SIZE,TILE_SIZE, BROWN);

                    if(map[1][i][j]>0) {
                        DrawTexture(GroundTile, i*TILE_SIZE+map0.x, j*TILE_SIZE+map0.y,kingdoms[map[1][i][j]].color);
                    }
                    else {
                        // Writing roadways' numbers on the tile
                        char innerNum[2];
                        sprintf(innerNum, "%d", map[0][i][j]);
                        DrawTextEx(font, innerNum,
                                   (Vector2) {(i + .4) * TILE_SIZE + map0.x, (j + .15) * TILE_SIZE + map0.y}, 30, 1,
                                   (Color) {150, 75, 0, 200});
                    }
            }

            // Hover Effect
            if(coordination.x==i && coordination.y==j) {
                DrawRectangle(i*TILE_SIZE + map0.x, j*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentWhite);
            }
        }
    }

    //Villages' Information
    for(int i=0; i<mapWidth; i++) {
        for(int j=0; j<mapHeight; j++) {
            if(coordination.x==i && coordination.y==j && map[0][i][j]==-2) {
                //show the shortest path
                int id= map[1][i][j];
                for(int k=0; villages[id].pathNumber > k; k++) {
                    DrawRectangle((villages[id].path[k] % mapWidth) * TILE_SIZE + map0.x, (villages[id].path[k] / mapWidth) * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentWhite);
                }
                //village info
                DrawRectangle((i - 1.3) * TILE_SIZE + map0.x, (j-0.2) * TILE_SIZE + map0.y, (1.7) * TILE_SIZE,(1.2) * TILE_SIZE, (Color) {56, 125, 55, 100});
                char goldFood[30];
                sprintf(goldFood, "%d gold\n%d food", villages[id].goldX, villages[id].foodX);
                DrawTextEx(font,goldFood,(Vector2){(i-1.3)*TILE_SIZE+map0.x,(j-0.1)*TILE_SIZE+map0.y}, 20,1,(Color){35, 97, 34, 200});
            }
        }
    }
    return 0;
}

int checkNeighbors(int x, int y, Vector2 map0) {
    Rectangle available;
    if(x!=0 && ((map[0][x-1][y]>0 && map[1][x-1][y]==0) || (map[0][x-1][y]==-2 && villages[map[1][x-1][y]].kingdom==0))) {
        DrawRectangle((x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        available= (Rectangle){(x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if(map[0][x-1][y] == -2) {
                villages[map[1][x-1][y]].kingdom = turn;
                kingdoms[turn].goldX += villages[map[1][x-1][y]].goldX;
                kingdoms[turn].foodX += villages[map[1][x-1][y]].foodX;
                printf("%d", map[1][x-1][y]);
            }

            else {
                map[1][x-1][y]=turn;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].x = x-1;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].y = y;
                kingdoms[turn].roadNumber++;
            }

            mode=0;
            turn++;
            return 1;
        }
    }
    if(x!=(mapWidth-1) && ((map[0][x+1][y]>0 && map[1][x+1][y]==0) || (map[0][x+1][y]==-2 && villages[map[1][x+1][y]].kingdom==0))) {
        DrawRectangle((x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        available= (Rectangle){(x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if(map[0][x+1][y] == -2) {
                villages[map[1][x+1][y]].kingdom = turn;
                kingdoms[turn].goldX += villages[map[1][x+1][y]].goldX;
                kingdoms[turn].foodX += villages[map[1][x+1][y]].foodX;
            }
            else {
                map[1][x+1][y]=turn;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].x = x+1;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].y = y;
                kingdoms[turn].roadNumber++;
            }

            mode=0;
            turn++;
            return 1;
        }
    }
    if(y!=0 && ((map[0][x][y-1]>0 && map[1][x][y-1]==0) || (map[0][x][y-1]==-2 && villages[map[1][x][y-1]].kingdom==0))) {
        DrawRectangle(x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        available= (Rectangle){x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if(map[0][x][y-1] == -2) {
                villages[map[1][x][y-1]].kingdom = turn;
                kingdoms[turn].goldX += villages[map[1][x][y-1]].goldX;
                kingdoms[turn].foodX += villages[map[1][x][y-1]].foodX;
            }
            else {
                map[1][x][y-1]=turn;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].x = x;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].y = y-1;
                kingdoms[turn].roadNumber++;
            }

            mode=0;
            turn++;
            return 1;
        }
    }
    if(y!=(mapHeight-1) && ((map[0][x][y+1]>0 && map[1][x][y+1]==0) || (map[0][x][y+1]==-2 && villages[map[1][x][y+1]].kingdom==0))) {
        DrawRectangle(x*TILE_SIZE + map0.x, (y+1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        available= (Rectangle){x*TILE_SIZE + map0.x, (y+1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if(map[0][x][y+1] == -2) {
                villages[map[1][x][y+1]].kingdom = turn;
                kingdoms[turn].goldX += villages[map[1][x][y+1]].goldX;
                kingdoms[turn].foodX += villages[map[1][x][y+1]].foodX;
            }
            else {
                map[1][x][y+1]=turn;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].x = x;
                kingdoms[turn].roads[kingdoms[turn].roadNumber].y = y+1;
                kingdoms[turn].roadNumber++;
            }

            mode=0;
            turn++;
            return 1;
        }
    }
    return 0;
}