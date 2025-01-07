#include <stdio.h>
#include "raylib.h"
#include "Map.h"
#include <string.h>

typedef struct kingdom kingdom;
typedef struct village village;

// Global Variables
int map[2][MAP_SIZE][MAP_SIZE];
int mapHeight = 0;
int mapWidth = 0;
int winner;
int turn = 1, kingdomNumber, villageNumber, opponent;
int dijkstraX, dijkstraY;
int list[MAP_SIZE*MAP_SIZE][5];
int mode = 0;
int MakeRoad = 0, roadX, roadY;
int animation=0, toBeDeleted = 0;

Vector2 mousePosition;
Vector2 coordination;
Vector2 manPos;

Color transparentWhite = (Color){255, 255, 255, 60};
Color transparentGreen = (Color){37, 204, 81, 90};
Color transparentRed = (Color){255, 0,0,60};

float buttonsPosY;
int action;

button buttons[5];
village villages[30] = {0};
kingdom kingdoms[6] = {0};

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

void initialMapMaker() {
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
}

void makeKingdom() {
    int x, y;
    printf("Enter the number of kingdoms:");
    scanf("%d", &kingdomNumber);
    while(kingdomNumber<1 || kingdomNumber>4) {
        printf("Too many kingdoms\nEnter the number of kingdoms:");
        scanf("%d", &kingdomNumber);
    }
    kingdoms[0].dead = 1;
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
}

void makeVillage() {
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
}

void makeBarrier() {
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
}

void mapDrawer(Texture2D mapTileSet, Texture2D GroundTile, Texture2D Castle, Texture2D House,Texture2D Stone, Font font, Vector2 map0) {
    DrawTexture(mapTileSet, 0,0, WHITE);
    for(int i=0; i<mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {

            DrawTexture(GroundTile, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, WHITE);
            DrawRectangleLines(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE,
                               (Color) {150, 75, 0, 100});

            switch (map[0][i][j]) {

                case -1: // -1 is the code for kingdoms.
                    DrawTexture(Castle, i * TILE_SIZE + map0.x, (j - 0.5) * TILE_SIZE + map0.y,
                                kingdoms[map[1][i][j]].color);
                    break;

                case -2: // -2 is the code for villages.
                    DrawTexture(House, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y,
                                kingdoms[villages[map[1][i][j]].kingdom].color);
                    break;

                case -3: // -3 is the code for barriers.
                    DrawTexture(Stone, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, WHITE);
                    break;

                default: // It is used for roadways' numbers.
                    DrawRectangleLines(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, BROWN);

                    if (map[1][i][j] > 0) {
                        DrawTexture(GroundTile, i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y,
                                    kingdoms[map[1][i][j]].color);
                    } else {
                        // Writing roadways' numbers on the tile
                        char innerNum[2];
                        sprintf(innerNum, "%d", kingdoms[turn].roadLeftover[i][j]);
                        DrawTextEx(font, innerNum,
                                   (Vector2) {(i + .4) * TILE_SIZE + map0.x, (j + .15) * TILE_SIZE + map0.y}, 30, 1,
                                   (Color) {150, 75, 0, 200});
                    }
            }

            // Hover Effect
            if (coordination.x == i && coordination.y == j) {
                DrawRectangle(i * TILE_SIZE + map0.x, j * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentWhite);
            }
        }
    }

    //Villages' Information
    for(int i=0; i<mapWidth; i++) {
        for(int j=0; j<mapHeight; j++) {
            if(coordination.x==i && coordination.y==j && map[0][i][j]==-2) {
                //show the shortest path
                int id = map[1][i][j];
                for (int k = 1; k < villages[id].pathNumber; k++) {
                    DrawRectangle((villages[id].path[k] % mapWidth) * TILE_SIZE + map0.x,
                                  (villages[id].path[k] / mapWidth) * TILE_SIZE + map0.y,
                                  TILE_SIZE, TILE_SIZE, transparentWhite);
                }
                //village info
                DrawRectangle((i - 1.3) * TILE_SIZE + map0.x, (j - 0.2) * TILE_SIZE + map0.y, (1.7) * TILE_SIZE,
                              (1.2) * TILE_SIZE, (Color) {56, 125, 55, 100});
                char goldFood[30];
                sprintf(goldFood, "%d gold\n%d food", villages[id].goldX, villages[id].foodX);
                DrawTextEx(font, goldFood, (Vector2) {(i - 1.3) * TILE_SIZE + map0.x, (j - 0.1) * TILE_SIZE + map0.y},
                           20, 1, (Color) {35, 97, 34, 200});

            }
        }
    }
}

int checkNeighbors(int x, int y, Vector2 map0) {
    Rectangle available;
    if(x != 0 && ((map[0][x-1][y] > 0 && map[1][x-1][y] == 0) ||
      (map[0][x-1][y] == -2 && villages[map[1][x-1][y]].kingdom == 0))) {
        int checker = checkForWar(x - 1, y);
        if (checker) {
            DrawRectangle((x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle((x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available = (Rectangle){(x-1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                MakeRoad = 1;
                roadX = x - 1;
                roadY = y;
            }
        }
    }
    if(x!=(mapWidth-1) && ((map[0][x+1][y]>0 && map[1][x+1][y]==0) ||
      (map[0][x+1][y]==-2 && villages[map[1][x+1][y]].kingdom==0))) {
        int checker = checkForWar(x + 1, y);
        if (checker) {
            DrawRectangle((x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle((x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available= (Rectangle){(x+1)*TILE_SIZE + map0.x, y*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                   MakeRoad = 1;
                   roadX = x + 1;
                   roadY = y;
            }
        }
    }
    if(y!=0 && ((map[0][x][y-1]>0 && map[1][x][y-1]==0) ||
      (map[0][x][y-1]==-2 && villages[map[1][x][y-1]].kingdom==0))) {
        int checker = checkForWar(x, y - 1);
        if (checker) {
            DrawRectangle(x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle(x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available = (Rectangle){x*TILE_SIZE + map0.x, (y-1)*TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    MakeRoad = 1;
                    roadX = x;
                    roadY = y - 1;
            }
        }
    }
    if(y!=(mapHeight-1) && ((map[0][x][y+1]>0 && map[1][x][y+1]==0) ||
      (map[0][x][y+1]==-2 && villages[map[1][x][y+1]].kingdom==0))) {
        int checker = checkForWar(x, y + 1);
        if (checker) {
            DrawRectangle(x * TILE_SIZE + map0.x, (y + 1) * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentRed);
        } else {
            DrawRectangle(x * TILE_SIZE + map0.x, (y + 1) * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE, transparentGreen);
        }
        available = (Rectangle) {x * TILE_SIZE + map0.x, (y + 1) * TILE_SIZE + map0.y, TILE_SIZE, TILE_SIZE};

        if (CheckCollisionPointRec(mousePosition, available)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                MakeRoad = 1;
                roadX = x;
                roadY = y + 1;
            }
        }
    }

    if (MakeRoad) {
        mode=4;
        animation = 1;
        manPos = (Vector2) {(roadX+.2) * TILE_SIZE + map0.x, (roadY-.2) * TILE_SIZE + map0.y};
        MakeRoad = 0;
        return 1;
    }
    return 0;
}

void RoadMaker() {
    kingdoms[turn].roadLeftover[roadX][roadY] -= kingdoms[turn].worker;
    if (kingdoms[turn].roadLeftover[roadX][roadY] <= 0) {
        kingdoms[turn].roadLeftover[roadX][roadY] = 0;
        map[1][roadX][roadY] = turn;
        kingdoms[turn].road[kingdoms[turn].roadNumber].x = roadX;
        kingdoms[turn].road[kingdoms[turn].roadNumber].y = roadY;
        kingdoms[turn].roadNumber++;

        for (int i = 0; i < 4; ++i) {
            int warType = checkForWar(roadX, roadY);
            Vector2 opponentV = {dijkstraX, dijkstraY};
            Vector2 turnV = {roadX, roadY};
            if (warType == 3) { // All-out war
                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                    toBeDeleted = opponent;
                    animation = 2;
                    mode = 4;
                    return;
                } else if (kingdoms[opponent].soldier > kingdoms[turn].soldier) {
                    toBeDeleted = turn;
                    animation = 2;
                    mode = 4;
                    return;
                } else {
                    normalWar(0, turn, 1, turnV);
                    normalWar(0, opponent, 1, turnV);
                }
            }
            else if (warType > 0) {
                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                    normalWar(turn, opponent, warType, opponentV);
                }
                else if (kingdoms[turn].soldier < kingdoms[opponent].soldier) {
                    normalWar(opponent, turn, warType, turnV);
                }
                else {
                    normalWar(0, opponent, warType, opponentV);
                    normalWar(0, turn, warType, turnV);
                }
            }

            int VillageID;
            if (map[0][roadX + 1][roadY] == -2 && (warType==0 || (warType>0 && kingdoms[turn].soldier>kingdoms[opponent].soldier))) {
                VillageID = map[1][roadX + 1][roadY];
                if (villages[VillageID].kingdom == 0) {
                    conquerVillage(VillageID, turn);
                }
            }
            if (map[0][roadX - 1][roadY] == -2 && (warType==0 || (warType>0 && kingdoms[turn].soldier>kingdoms[opponent].soldier))) {
                VillageID = map[1][roadX - 1][roadY];
                if (villages[VillageID].kingdom == 0) {
                    conquerVillage(VillageID, turn);
                }
            }
            if (map[0][roadX][roadY - 1] == -2 && (warType==0 || (warType>0 && kingdoms[turn].soldier>kingdoms[opponent].soldier))) {
                VillageID = map[1][roadX][roadY - 1];
                if (villages[VillageID].kingdom == 0) {
                    conquerVillage(VillageID, turn);
                }
            }
            if (map[0][roadX][roadY + 1] == -2 && (warType==0 || (warType>0 && kingdoms[turn].soldier>kingdoms[opponent].soldier))) {
                VillageID = map[1][roadX][roadY + 1];
                if (villages[VillageID].kingdom == 0) {
                    conquerVillage(VillageID, turn);
                }
            }
        }
    }


    mode=0;
    turn++;
}

void mode0() {

    do {
        if (turn > kingdomNumber) {
            turn = 1;
            for (int i = 1; i <= kingdomNumber; i++) {
                if (kingdoms[i].dead)
                    continue;
                kingdoms[i].food += kingdoms[i].foodX;
                kingdoms[i].gold += kingdoms[i].goldX;
            }
        }
        if (kingdoms[turn].dead) turn++;
    } while(kingdoms[turn].dead);

    int kingdomVertexNumber = kingdoms[turn].y*mapWidth + kingdoms[turn].x;
    for(int i=0; i<villageNumber; i++) {
        dijkstraPath(kingdomVertexNumber, i, mapWidth*mapHeight);
    }

    //choose the shortest path
    for (int i = 0; i < villageNumber; ++i) {
        int BestPath = kingdoms[turn].y * mapWidth + kingdoms[turn].x, path, distance
        , BestDistance =dijkstraPath(kingdomVertexNumber, i, mapWidth * mapHeight);

        for (int j = 0; j < kingdoms[turn].roadNumber; ++j) {
            path = kingdoms[turn].road[j].y * mapWidth + kingdoms[turn].road[j].x;
            distance = dijkstraPath(path, i , mapWidth * mapHeight);
            if (dijkstraPath(path, i , mapWidth * mapHeight) <= BestDistance) {
                BestPath = path;
                BestDistance = distance;
            }
        }
        for (int j=0; j<villageNumber; ++j) {
            if (villages[j].kingdom == turn) {
                path = villages[j].y * mapWidth + villages[j].x;
                distance = dijkstraPath(path, i , mapWidth * mapHeight);
                if (dijkstraPath(path, i , mapWidth * mapHeight) < BestDistance) {
                    BestPath = path;
                    BestDistance = distance;
                }
            }
        }

        dijkstraPath(BestPath, i , mapWidth * mapHeight);
    }

    strcpy(buttons[0].text, "Add food");
    strcpy(buttons[1].text, "Add worker");
    strcpy(buttons[2].text, "Add soldier");
    strcpy(buttons[3].text, "Make road");
    strcpy(buttons[4].text, "Do nothing");
    for (int i = 0; i < 5; ++i) {
        buttons[i].rect.x = 30 + 175 * i;
        if (buttonsPosY < 20) buttonsPosY += GetFrameTime() * 35;
        buttons[i].rect.y = buttonsPosY;
        buttons[i].rect.width = 150;
        buttons[i].rect.height = 65;
        buttons[i].color = BLACK;
        if (CheckCollisionPointRec(mousePosition, buttons[i].rect)) {
            buttons[i].color = Fade(BLACK, 0.6f);
            buttons[i].rect.x += 2;
            buttons[i].rect.y += 2;
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                mode = 1;
                action = i + 1;
            }
        }

        // Draw shadow for 3D effect
        DrawRectangleRounded((Rectangle){buttons[i].rect.x + 2, buttons[i].rect.y + 2, buttons[i].rect.width, buttons[i].rect.height},
                             .3f, 16, buttons[i].color);

        DrawRectangleRounded(buttons[i].rect, .3f, 16, buttons[i].color);

        // Draw button label
        int textWidth = MeasureText(buttons[i].text, 20);
        Vector2 textPos = {buttons[i].rect.x + buttons[i].rect.width / 2 - textWidth / 2,
                           buttons[i].rect.y + buttons[i].rect.height / 2 - 10};
        DrawText(buttons[i].text, textPos.x, textPos.y, 20, kingdoms[turn].color);
    }
}

void mode1() {
    buttonsPosY = -100;
    switch (action) {
        case 1:
            if (kingdoms[turn].gold >= 1) {
                kingdoms[turn].gold--;
                kingdoms[turn].food++;

            } else {
                DrawRectangle(90, 90, 710, 40, (Color) {222, 131, 124, 100});
                DrawText("You don't have enough gold to buy food!!!", 100, 100, 30, RED);
                EndDrawing();
                WaitTime(2);
                turn--;
            }

            break;

        case 2:
            if (kingdoms[turn].food >= 3) {
                kingdoms[turn].food -= 3;
                kingdoms[turn].worker++;
            } else {
                DrawRectangle(90, 90, 720, 40, (Color) {222, 131, 124, 100});
                DrawText("You don't have enough food to hire workers!!!", 100, 100, 30, RED);
                EndDrawing();
                WaitTime(1.75);
                turn--;
            }
            break;

        case 3:
            if (kingdoms[turn].gold >= 2) {
                kingdoms[turn].gold -= 2;
                kingdoms[turn].soldier++;
            } else {
                DrawRectangle(90, 90, 720, 40, (Color) {222, 131, 124, 100});
                DrawText("You don't have enough gold to hire soldiers!!!", 100, 100, 30, RED);
                EndDrawing();
                WaitTime(1.75);
                turn--;
            }
            break;

        case 4:
            mode=2;
        case 5:
            break;
    }
    if (mode==1) {
        mode = 0;
        turn++;
    }
}

