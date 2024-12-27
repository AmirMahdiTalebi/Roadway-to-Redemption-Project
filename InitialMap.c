#include <stdio.h>
#include "raylib.h"
#include "InitialMap.h"

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
int map[2][MAP_SIZE][MAP_SIZE];
int mapHeight = 0;
int mapWidth = 0;
int winner;
int turn = 1, kingdomNumber, villageNumber, neededSoldier, opponent;
int list[MAP_SIZE*MAP_SIZE][5];
int mode = 0;

Vector2 mousePosition;
Vector2 coordination;

Color transparentWhite = (Color){255, 255, 255, 60};
Color transparentGreen = (Color){37, 204, 81, 90};
Color transparentRed = (Color){255, 0,0,60};

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
                //anything that can't become a road
                list[j*mapWidth+i][0] = 2000;
            }
            else {
                if (map[1][i][j]!=turn && map[1][i][j]>0)
                    list[j*mapWidth + i][0]=2000;
                else
                    list[j * mapWidth + i][0] = (kingdoms[turn].roadLeftover[i][j] + kingdoms[turn].worker - 1) / kingdoms[turn].worker;
            }
        }
    }
    list[source][0] = list[dest][0] = 0;

    //Adding neighbors to the list
    for (int i = 0; i < mapWidth*mapHeight; ++i) {
        int k = 0;
        int e = 0;
        int vertexIndex = i + 1;
        if ((vertexIndex - mapWidth) <= 0) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - mapWidth;
            k++;
        }
        if ((vertexIndex + mapWidth) > (mapWidth * mapHeight)) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i + mapWidth;
            k++;
        }
        if (vertexIndex % mapWidth == 1) {
            list[i][4-e] = -1;
            e++;
        }
        else {
            list[i][1+k] = i - 1;
            k++;
        }
        if ((vertexIndex) % mapWidth == 0) list[i][4-e] = -1;
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
        minDistance=4000;
        for(int i=0; i<size; i++) {
            if(!visited[i] && dist[i]<minDistance && list[i][0] != -1) {
                current=i;
                minDistance=dist[i];
            }
        }
    }

    int k;
    for (k = 0; k < pathNumber[dest]; k++) {
        villages[id].path[k] = path[dest][k];
    }
    villages[id].pathNumber = pathNumber[dest];
    return dist[dest];
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
            map[1][i][j] = 0;
        }
    }
    return 0;
}

int makeKingdom() {
    int x, y;
    printf("Enter the number of kingdoms:");
    scanf("%d", &kingdomNumber);
    for (int i = 1; i <= kingdomNumber; ++i) {
        printf("Enter x and y for the kingdom No.%d:\n", i);
        scanf("%d %d", &x, &y);
        while(x<=0 || x>mapWidth || y<=0 || y>mapHeight) {
            printf("x and y are invalid.\nEnter x and y for the kingdom No.%d:", i);
            scanf("%d %d", &x, &y);
        }
        while(map[0][x-1][y-1] <= 0) {
            printf("There is already something else there.\nEnter x and y for the kingdom No.%d:", i);
            scanf("%d %d", &x, &y);
        }
        //x and y start from 1
        x--;
        y--;
        kingdoms[i].x = x;
        kingdoms[i].y = y;
        map[0][x][y] = -1; // -1 is the code for kingdoms.
        map[1][x][y] = i; // I saved Kingdoms' IDs in z=1.
        kingdoms[i].gold = 5;
        kingdoms[i].food = kingdoms[i].foodX = kingdoms[i].soldier = kingdoms[i].villageNumber = kingdoms[i].dead = 0;
        kingdoms[i].goldX = kingdoms[i].worker = 1;
        for (int mapX = 0; mapX < mapWidth; ++mapX) {
            for (int mapY = 0; mapY < mapHeight; ++mapY) {
                kingdoms[i].roadLeftover[mapX][mapY] = map[0][mapX][mapY];
            }
        }
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
                        sprintf(innerNum, "%d", kingdoms[turn].roadLeftover[i][j]);
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
                for(int k=1; k < villages[id].pathNumber; k++) {
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
    if(x != 0 && ((map[0][x-1][y] > 0 && map[1][x-1][y] == 0) || (map[0][x-1][y] == -2 && villages[map[1][x-1][y]].kingdom == 0))) {
        int checker = checkForWar(x-1 ,y ,map0);
        if (checker) {
            DrawRectangle((x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle((x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available = (Rectangle){(x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    kingdoms[turn].roadLeftover[x - 1][y] -= kingdoms[turn].worker;
                    if (kingdoms[turn].roadLeftover[x - 1][y] <= 0) {
                        kingdoms[turn].roadLeftover[x - 1][y] = 0;
                        map[1][x - 1][y] = turn;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].x = x - 1;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].y = y;
                        kingdoms[turn].roadNumber++;
                        if (map[0][x - 1 + 1][y] == -2 || map[0][x - 1 - 1][y] == -2 ||
                            map[0][x - 1][y - 1] == -2 || map[0][x - 1][y + 1] == -2) {
                            int VID = (map[0][x - 1 + 1][y] == -2) * map[1][x - 1 + 1][y]
                                      + (map[0][x - 1 - 1][y] == -2) * map[1][x - 1 - 1][y]
                                      + (map[0][x - 1][y - 1] == -2) * map[1][x - 1][y - 1]
                                      + (map[0][x - 1][y + 1] == -2) * map[1][x - 1][y + 1];
                            villages[VID].kingdom = turn;
                            kingdoms[turn].villageNumber++;
                            kingdoms[turn].goldX += villages[VID].goldX;
                            kingdoms[turn].foodX += villages[VID].foodX;
                        }


                        if (checker) { //war
                            if (map[0][x - 1 + 1][y] == -1 || map[0][x - 1 - 1][y] == -1 ||
                                map[0][x - 1][y - 1] == -1 || map[0][x - 1][y + 1] == -1) //all-out war
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    DeleteKingdom(opponent);
                                    return 1;
                                }
                                else {
                                    DeleteKingdom(turn);
                                    return 1;
                                }
                            else {
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    kingdoms[turn].soldier -= kingdoms[opponent].soldier;
                                    kingdoms[opponent].soldier = 0;
                                }
                                else {
                                    kingdoms[opponent].soldier -= kingdoms[turn].soldier;
                                    kingdoms[turn].soldier = 0;
                                }
                            }
                        }
                    }

                mode = 0;
                turn++;
                return 1;
            }
        }
    }
    if(x!=(mapWidth-1) && ((map[0][x+1][y]>0 && map[1][x+1][y]==0) || (map[0][x+1][y]==-2 && villages[map[1][x+1][y]].kingdom==0))) {
        int checker = checkForWar(x+1 ,y ,map0);
        if (checker) {
            DrawRectangle((x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle((x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available= (Rectangle){(x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    kingdoms[turn].roadLeftover[x + 1][y] -= kingdoms[turn].worker;
                    if (kingdoms[turn].roadLeftover[x + 1][y] <= 0) {
                        kingdoms[turn].roadLeftover[x + 1][y] = 0;
                        map[1][x+1][y] = turn;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].x = x + 1;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].y = y;
                        kingdoms[turn].roadNumber++;
                        if(map[0][x + 1 + 1][y] == -2 || map[0][x + 1 - 1][y] == -2 ||
                           map[0][x + 1][y - 1] == -2 || map[0][x + 1][y + 1] == -2) {
                            int VID = (map[0][x + 1 + 1][y] == -2) * map[1][x + 1 + 1][y]
                                      + (map[0][x + 1 - 1][y] == -2) * map[1][x + 1 - 1][y]
                                      + (map[0][x + 1][y - 1] == -2) * map[1][x + 1][y - 1]
                                      + (map[0][x + 1][y + 1] == -2) * map[1][x + 1][y + 1];
                            villages[VID].kingdom = turn;
                            kingdoms[turn].villageNumber++;
                            kingdoms[turn].goldX += villages[VID].goldX;
                            kingdoms[turn].foodX += villages[VID].foodX;
                        }
                        if (checker) { //war
                            if (map[0][x + 1 + 1][y] == -1 || map[0][x + 1 - 1][y] == -1 ||
                                map[0][x + 1][y - 1] == -1 || map[0][x + 1][y + 1] == -1) //all-out war
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    DeleteKingdom(opponent);
                                    return 1;
                                }
                                else {
                                    DeleteKingdom(turn);
                                    return 1;
                                }
                            else {
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    kingdoms[turn].soldier -= kingdoms[opponent].soldier;
                                    kingdoms[opponent].soldier = 0;
                                }
                                else {
                                    kingdoms[opponent].soldier -= kingdoms[turn].soldier;
                                    kingdoms[turn].soldier = 0;
                                }
                            }
                        }
                    }

                mode=0;
                turn++;
                return 1;
            }
        }
    }
    if(y!=0 && ((map[0][x][y-1]>0 && map[1][x][y-1]==0) || (map[0][x][y-1]==-2 && villages[map[1][x][y-1]].kingdom==0))) {
        int checker = checkForWar(x ,y-1 ,map0);
        if (checker) {
            DrawRectangle(x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle(x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available = (Rectangle){x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    kingdoms[turn].roadLeftover[x][y - 1] -= kingdoms[turn].worker;
                    if (kingdoms[turn].roadLeftover[x][y - 1] <= 0) {
                        kingdoms[turn].roadLeftover[x][y - 1] = 0;
                        map[1][x][y-1] = turn;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].x = x;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].y = y - 1;
                        kingdoms[turn].roadNumber++;
                        if(map[0][x + 1][y - 1] == -2 || map[0][x - 1][y - 1] == -2 ||
                           map[0][x][y - 1 - 1] == -2 || map[0][x][y - 1 + 1] == -2) {
                            int VID = (map[0][x + 1][y - 1] == -2) * map[1][x + 1][y - 1]
                                      + (map[0][x- 1][y - 1] == -2) * map[1][x - 1][y - 1]
                                      + (map[0][x][y - 1 - 1] == -2) * map[1][x][y - 1 - 1]
                                      + (map[0][x][y - 1 + 1] == -2) * map[1][x][y - 1 + 1];
                            villages[VID].kingdom = turn;
                            kingdoms[turn].villageNumber++;
                            kingdoms[turn].goldX += villages[VID].goldX;
                            kingdoms[turn].foodX += villages[VID].foodX;
                        }
                        if (checker) { //war
                            if (map[0][x + 1][y - 1] == -1 || map[0][x - 1][y - 1] == -1 ||
                                map[0][x][y - 1 - 1] == -1 || map[0][x][y - 1 + 1] == -1) //all-out war
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    DeleteKingdom(opponent);
                                    return 1;
                                }
                                else {
                                    DeleteKingdom(turn);
                                    return 1;
                                }
                            else {
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    kingdoms[turn].soldier -= kingdoms[opponent].soldier;
                                    kingdoms[opponent].soldier = 0;
                                }
                                else {
                                    kingdoms[opponent].soldier -= kingdoms[turn].soldier;
                                    kingdoms[turn].soldier = 0;
                                }
                            }
                        }
                    }

                mode=0;
                turn++;
                return 1;
            }
        }
    }
    if(y!=(mapHeight-1) && ((map[0][x][y+1]>0 && map[1][x][y+1]==0) || (map[0][x][y+1]==-2 && villages[map[1][x][y+1]].kingdom==0))) {
        int checker = checkForWar(x ,y+1 ,map0);
        if (checker) {
            DrawRectangle(x*TILE_SIZE + map0.x, (y+1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle(x*TILE_SIZE + map0.x, (y+1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available= (Rectangle){x*TILE_SIZE + map0.x, (y+1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    kingdoms[turn].roadLeftover[x][y + 1] -= kingdoms[turn].worker;
                    if (kingdoms[turn].roadLeftover[x][y + 1] <= 0) {
                        kingdoms[turn].roadLeftover[x][y + 1] = 0;
                        map[1][x][y+1] = turn;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].x = x;
                        kingdoms[turn].road[kingdoms[turn].roadNumber].y = y + 1;
                        kingdoms[turn].roadNumber++;
                        if(map[0][x + 1][y + 1] == -2 || map[0][x - 1][y + 1] == -2 ||
                           map[0][x][y + 1 - 1] == -2 || map[0][x][y + 1 + 1] == -2) {
                            int VID = (map[0][x + 1][y + 1] == -2) * map[1][x + 1][y + 1]
                                      + (map[0][x- 1][y + 1] == -2) * map[1][x - 1][y + 1]
                                      + (map[0][x][y + 1 - 1] == -2) * map[1][x][y + 1 - 1]
                                      + (map[0][x][y + 1 + 1] == -2) * map[1][x][y + 1 + 1];
                            villages[VID].kingdom = turn;
                            kingdoms[turn].villageNumber++;
                            kingdoms[turn].goldX += villages[VID].goldX;
                            kingdoms[turn].foodX += villages[VID].foodX;
                        }
                        if (checker) { //war
                            if (map[0][x + 1][y - 1] == -1 || map[0][x - 1][y - 1] == -1 ||
                                map[0][x][y - 1 - 1] == -1 || map[0][x][y - 1 + 1] == -1) //all-out war
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    DeleteKingdom(opponent);
                                    return 1;
                                }
                                else {
                                    DeleteKingdom(turn);
                                    return 1;
                                }
                            else {
                                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                                    kingdoms[turn].soldier -= kingdoms[opponent].soldier;
                                    kingdoms[opponent].soldier = 0;
                                }
                                else {
                                    kingdoms[opponent].soldier -= kingdoms[turn].soldier;
                                    kingdoms[turn].soldier = 0;
                                }
                            }
                        }
                    }

                mode=0;
                turn++;
                return 1;
            }
        }
    }
    return 0;
}

int checkForWar(int x, int y, Vector2 map0) {
    if ((x != 0 && ((map[0][x-1][y] > 0 && map[1][x-1][y] != 0 && map[1][x-1][y] != turn) ||
                   (map[0][x-1][y] == -1 && map[1][x-1][y] != turn) ||
                   (map[0][x-1][y] == -2 && villages[map[1][x-1][y]].kingdom != 0 && villages[map[1][x-1][y]].kingdom != turn)))
                   && !kingdoms[map[1][x-1][y]].dead){
        opponent = map[1][x-1][y];
        return 1;
    }

    if ((y != 0 && ((map[0][x][y-1] > 0 && map[1][x][y-1] != 0 && map[1][x][y-1] != turn) ||
                   (map[0][x][y-1] == -1 && map[1][x][y-1] != turn) ||
                   (map[0][x][y-1] == -2 && villages[map[1][x][y-1]].kingdom != 0 && villages[map[1][x][y-1]].kingdom != turn)))
                   && !kingdoms[map[1][x][y-1]].dead){
        opponent = map[1][x][y-1];
        return 1;
    }

    if ((y != (mapHeight-1) && ((map[0][x][y+1] > 0 && map[1][x][y+1] != 0 && map[1][x][y+1] != turn) ||
                   (map[0][x][y+1] == -1 && map[1][x][y+1] != turn) ||
                   (map[0][x][y+1] == -2 && villages[map[1][x][y+1]].kingdom != 0 && villages[map[1][x][y+1]].kingdom != turn)))
                   && !kingdoms[map[1][x][y+1]].dead){
        opponent = map[1][x][y+1];
        return 1;
    }

    if ((x != (mapWidth-1) && ((map[0][x+1][y] > 0 && map[1][x+1][y] != 0 && map[1][x+1][y] != turn) ||
                   (map[0][x+1][y] == -1 && map[1][x+1][y] != turn) ||
                   (map[0][x+1][y] == -2 && villages[map[1][x+1][y]].kingdom != 0 && villages[map[1][x+1][y]].kingdom != turn)))
                   && !kingdoms[map[1][x+1][y]].dead){
        opponent = map[1][x+1][y];
        return 1;
    }
    return 0;
}

int DeleteKingdom(int id) {
    kingdoms[id].food = 0;
    kingdoms[id].foodX = 0;
    kingdoms[id].gold = 0;
    kingdoms[id].goldX = 0;
    kingdoms[id].worker = 0;
    kingdoms[id].soldier = 0;
    for (int i = 0; i < kingdoms[id].roadNumber; ++i) {
        map[1][(int)kingdoms[id].road[i].x][(int)kingdoms[id].road[i].y] = 0;
    }
    kingdoms[id].roadNumber=0;
    for (int i = 0; i < villageNumber; ++i) {
        if (villages[i].kingdom  == id)
            villages[i].kingdom = 0;
    }
    kingdoms[id].villageNumber = 0;
    kingdoms[id].color = BLACK;
    kingdoms[id].dead=1;

    int aliveCount=0, alive=0;
    for (int i = 1; i <= kingdomNumber; ++i) {
        aliveCount += !kingdoms[i].dead;
        alive += i * !kingdoms[i].dead;
    }
    if (aliveCount == 1) {
        turn++;
        winner = alive;
        mode = 3;
    }
    else {
        turn++;
        mode = 0;
    }
    return 0;
}