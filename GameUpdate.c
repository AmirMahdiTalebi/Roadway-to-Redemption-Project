#include <string.h>
#include "GameUpdate.h"
#include "InitialGame.h"
#include "War.h"
#include "monteCarlo.h"
#include <stdio.h>

int dijkstraVillage(int source, int id, int size) {
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

int checkNeighbors(int x, int y) {
    Rectangle available;
    int availableNumber = kingdoms[turn].availableNumber;
    if(x != 0 && (map[0][x-1][y] > 0 && map[1][x-1][y] == 0)) {
        int isAlreadyAdded = 0;
        for (int i = 0; i < availableNumber && !isAlreadyAdded; ++i) {
            if (kingdoms[turn].available[i].x == x-1 && kingdoms[turn].available[i].y == y) isAlreadyAdded = 1;
        }
        if (!isAlreadyAdded) {
            kingdoms[turn].available[availableNumber].x = x-1;
            kingdoms[turn].available[availableNumber].y = y;
            kingdoms[turn].availableNumber++;
            availableNumber++;
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
    if(x!=(mapWidth-1) && (map[0][x+1][y]>0 && map[1][x+1][y]==0)) {
        int isAlreadyAdded = 0;
        for (int i = 0; i < availableNumber && !isAlreadyAdded; ++i) {
            if (kingdoms[turn].available[i].x == x+1 && kingdoms[turn].available[i].y == y) isAlreadyAdded = 1;
        }
        if (!isAlreadyAdded) {
            kingdoms[turn].available[availableNumber].x = x+1;
            kingdoms[turn].available[availableNumber].y = y;
            kingdoms[turn].availableNumber++;
            availableNumber++;
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
    if(y!=0 && (map[0][x][y-1]>0 && map[1][x][y-1]==0)) {
        int isAlreadyAdded = 0;
        for (int i = 0; i < availableNumber && !isAlreadyAdded; ++i) {
            if (kingdoms[turn].available[i].x == x && kingdoms[turn].available[i].y == y-1) isAlreadyAdded = 1;
        }
        if (!isAlreadyAdded) {
            kingdoms[turn].available[availableNumber].x = x;
            kingdoms[turn].available[availableNumber].y = y-1;
            kingdoms[turn].availableNumber++;
            availableNumber++;
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
    if(y!=(mapHeight-1) && (map[0][x][y+1]>0 && map[1][x][y+1]==0)) {
        int isAlreadyAdded = 0;
        for (int i = 0; i < availableNumber && !isAlreadyAdded; ++i) {
            if (kingdoms[turn].available[i].x == x && kingdoms[turn].available[i].y == y+1) isAlreadyAdded = 1;
        }
        if (!isAlreadyAdded) {
            kingdoms[turn].available[availableNumber].x = x;
            kingdoms[turn].available[availableNumber].y = y+1;
            kingdoms[turn].availableNumber++;
            availableNumber++;
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
        int warType=0, i=0;
        while (checkForWar(roadX, roadY) && i++<4) {
            warType = checkForWar(roadX, roadY);
            Vector2 opponentV = {opponentX, opponentY};
            Vector2 turnV = {roadX, roadY};
            if (warType == 3) { // All-out war
                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                    toBeDeleted = opponent;
                    if (isPlayingWithMonte && turn == 2) {
                        DeleteKingdom(toBeDeleted);
                        return;
                    }
                    else
                        animation = 2;
                    mode = 4;
                    return;
                } else if (kingdoms[opponent].soldier > kingdoms[turn].soldier) {
                    toBeDeleted = turn;
                    if (isPlayingWithMonte && turn == 2) {
                        DeleteKingdom(toBeDeleted);
                        return;
                    }
                    else
                        animation = 2;
                    mode = 4;
                    return;
                } else {
                    normalWar(0, turn, 1, turnV);
                    normalWar(0, opponent, 1, turnV);
                }
            } else if (warType > 0) {
                if (kingdoms[turn].soldier > kingdoms[opponent].soldier) {
                    normalWar(turn, opponent, warType, opponentV);
                } else if (kingdoms[turn].soldier < kingdoms[opponent].soldier) {
                    normalWar(opponent, turn, warType, turnV);
                } else {
                    normalWar(0, opponent, warType, opponentV);
                    normalWar(0, turn, warType, turnV);
                }
            }
        }
        for (i = 0; i < 4; ++i) {
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
    if (kingdoms[1].dead) {
        buttonsPosY = -100;
        mode3();
        isPlayingWithMonte = 0;
    }
    if (turn == 2 && isPlayingWithMonte) {
        int monteAction = monte();
        char actionText[50] = " ";
        switch (monteAction) {
            case 1:
                strcpy(actionText, "The computer bought food");
                break;
            case 2:
                strcpy(actionText, "The computer hired a worker");
                break;
            case 3:
                strcpy(actionText, "The computer hired a soldier.");
                break;
            case 5:
                strcpy(actionText, "The computer did nothing");
                break;
            default: strcpy(actionText, "The computer built a road");
        }
        DrawRectangle(90, 90, 720, 40, (Color) {222, 131, 124, 100});
        int textWidth = MeasureText(actionText, 30);
        DrawText(actionText, (SCREEN_WIDTH - 300) / 2 - textWidth / 2, 100, 30, kingdoms[2].color);
        EndDrawing();
        WaitTime(1.5);
    }
    do {
        if (turn > kingdomNumber) {
            turn = 1;
            for (int i = 1; i <= kingdomNumber; i++) {
                if (!kingdoms[i].dead)
                    continue;
                kingdoms[i].food += kingdoms[i].foodX;
                kingdoms[i].gold += kingdoms[i].goldX;
            }
        }
        if (kingdoms[turn].dead) turn++;
    } while(kingdoms[turn].dead || turn> kingdomNumber);


    int kingdomVertexNumber = kingdoms[turn].y*mapWidth + kingdoms[turn].x;
    for(int i=0; i<villageNumber; i++) {
        dijkstraVillage(kingdomVertexNumber, i, mapWidth * mapHeight);
    }

    //choose the shortest path
    for (int i = 0; i < villageNumber; ++i) {
        int BestPath = kingdoms[turn].y * mapWidth + kingdoms[turn].x, path, distance
        , BestDistance = dijkstraVillage(kingdomVertexNumber, i, mapWidth * mapHeight);

        for (int j = 0; j < kingdoms[turn].roadNumber; ++j) {
            path = kingdoms[turn].road[j].y * mapWidth + kingdoms[turn].road[j].x;
            distance = dijkstraVillage(path, i, mapWidth * mapHeight);
            if (dijkstraVillage(path, i, mapWidth * mapHeight) <= BestDistance) {
                BestPath = path;
                BestDistance = distance;
            }
        }
        for (int j=0; j<villageNumber; ++j) {
            if (villages[j].kingdom == turn) {
                path = villages[j].y * mapWidth + villages[j].x;
                distance = dijkstraVillage(path, i, mapWidth * mapHeight);
                if (dijkstraVillage(path, i, mapWidth * mapHeight) < BestDistance) {
                    BestPath = path;
                    BestDistance = distance;
                }
            }
        }

        dijkstraVillage(BestPath, i, mapWidth * mapHeight);
    }

    strcpy(buttons[0].text, "Add food");
    strcpy(buttons[1].text, "Add worker");
    strcpy(buttons[2].text, "Add soldier");
    strcpy(buttons[3].text, "Make road");
    strcpy(buttons[4].text, "Do nothing");
    for (int i = 0; i < 5; ++i) {
        buttons[i].rect.x = 30 + 175 * i;
        if (isPlayingWithMonte) buttonsPosY = 20;
        else if (buttonsPosY < 20) buttonsPosY += GetFrameTime() * 35;
        buttons[i].rect.y = buttonsPosY;
        buttons[i].rect.width = 150;
        buttons[i].rect.height = 65;
        buttons[i].color = BLACK;
        if (CheckCollisionPointRec(mousePosition, buttons[i].rect)) {
            buttons[i].color = Fade(BLACK, 0.7f);
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
    if (!isPlayingWithMonte) buttonsPosY = -100;
    switch (action) {
        case 1:
            kingdoms[turn].gold--;
            kingdoms[turn].food++;
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

void mode3() {
    DrawRectangle(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){128, 128, 128, 150});
    char text[24];
    sprintf(text, "The winner Is Kingdom %d", winner);
    int textWidth = MeasureText(text, 40);
    DrawText(text, (SCREEN_WIDTH-300) / 2 - textWidth / 2,100, 40, kingdoms[winner].color);
}

