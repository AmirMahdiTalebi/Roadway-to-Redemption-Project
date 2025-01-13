#include "monteCarlo.h"
#include "Map.h"
#include "War.h"
#include "raylib.h"
#include "stdlib.h"
#include <math.h>
#include "stdio.h"
#include <limits.h>

#define constant 1.5
#define iterations 200

node root;

typedef struct node node;

void monte() {
    monteCarlo = 1;
    node *current;
    root.state = (gameState*) malloc(sizeof(gameState));
    root.parent = NULL;
    root.children = NULL;
    SaveGame(root.state);
    root.visits=root.winCount=0;
    current = &root;

    for (int i = 0; i < iterations ; ++i) {
        expand(current);
        for (int j = 0; j < current->childCount; ++j) {
            int gameResult = simulation(current->children[j]->state);
            backpropagation(current->children[j],gameResult);
        }
        current = selection();
    }

    gameState bestMove;
    int maxSimulations = -1;
    for (int i = 0; i < root.childCount; i++) {
        if (root.children[i]->visits > maxSimulations) {
            maxSimulations = root.children[i]->visits;
            bestMove = *root.children[i]->state;
        }
        printf("%d visits\n", root.children[i]->visits);
    }
    printf("\n");
    LoadGame(&bestMove);
    monteCarlo = 0;
    mode = 0;
    //freeTree(&root);
}

node* selection() {
    node* best;
    double maxUCB;
    node* parent;
    parent = (node*) malloc(sizeof(node));
    *parent = root;
    while (parent->children) {
        maxUCB = -1;
        for (int i = 0; i < parent->childCount; ++i) {
            double ucb;
            if (parent->children[i]->visits != 0) {
                ucb = ((parent->children[i]->winCount/parent->children[i]->visits) +
                       constant* sqrt(log(root.visits)/parent->children[i]->visits));
            } else {
                ucb = INT_MAX;
            }
            if (ucb > maxUCB && !parent->children[i]->state->end) {
                maxUCB = ucb;
                best = parent->children[i];
            }
        }
        parent = best;
    }
    return best;
}

void expand(node* parent) {
    parent->childCount = possibleMoves(parent->state);
    parent->children = (node**)malloc(parent->childCount * sizeof(node*));

    for (int i = 0; i < parent->childCount; i++) {
        parent->children[i] = (node *) malloc(sizeof(node));
        parent->children[i]->state = (gameState *) malloc(sizeof(gameState));
        *parent->children[i]->state = *parent->state;
        parent->children[i]->parent = parent;
        parent->children[i]->children = NULL;
        parent->children[i]->childCount = 0;
        parent->children[i]->winCount = 0;
        parent->children[i]->visits = 0;

        LoadGame(parent->children[i]->state);
        int move = i + 1;
        if (move <= kingdoms[turn].availableNumber) {
            roadX = kingdoms[turn].available[move - 1].x;
            roadY = kingdoms[turn].available[move - 1].y;
            RoadMaker();
        } else {
            if (move - kingdoms[turn].availableNumber == 1) { //add food
                if (kingdoms[turn].foodX < 3 && kingdoms[turn].worker < 4) {
                    kingdoms[turn].gold--;
                    kingdoms[turn].food++;
                    turn++;
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 2) { //add worker
                if (kingdoms[turn].worker < 4 && kingdoms[turn].food >= 3) {
                    kingdoms[turn].food -= 3;
                    kingdoms[turn].worker++;
                    turn++;
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 3) { //add soldier
                if (kingdoms[turn].gold >= 2) {
                    kingdoms[turn].gold -= 2;
                    kingdoms[turn].soldier++;
                    turn++;
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 4) { //do nothing
                turn++;
            }
        }
        if (turn > kingdomNumber) {
            turn = 1;
            for (int k = 1; k <= kingdomNumber; k++) {
                kingdoms[k].food += kingdoms[k].foodX;
                kingdoms[k].gold += kingdoms[k].goldX;
            }
        }
        SaveGame(parent->children[i]->state);
        if (mode == 3)
            parent->children[i]->state->end = 1;
    }
}

int simulation(gameState* state) {
    LoadGame(state);
    gameState *save = (gameState*) malloc(sizeof(gameState));
    *save = *state;
    for (int k = 0; k < mapWidth*mapHeight && !winner; ++k)  {
        if (turn > kingdomNumber) {
            turn = 1;
            for (int i = 1; i <= kingdomNumber; i++) {
                kingdoms[i].food += kingdoms[i].foodX;
                kingdoms[i].gold += kingdoms[i].goldX;
            }
        }
        SaveGame(save);
        int move = GetRandomValue(1, possibleMoves(save));

        if (move <= kingdoms[turn].availableNumber) {
            roadX = kingdoms[turn].available[move - 1].x;
            roadY = kingdoms[turn].available[move - 1].y;
            RoadMaker();

        } else {
            if (move - kingdoms[turn].availableNumber == 1) { //add food
                if (kingdoms[turn].foodX < 3 && kingdoms[turn].worker < 4) {
                    kingdoms[turn].gold--;
                    kingdoms[turn].food++;
                    turn++;
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 2) { //add worker
                if (kingdoms[turn].worker < 4 && kingdoms[turn].food >= 3) {
                    kingdoms[turn].food -= 3;
                    kingdoms[turn].worker++;
                    turn++;
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 3) {
                if (kingdoms[turn].gold >= 2) { //add soldier
                    kingdoms[turn].gold -= 2;
                    kingdoms[turn].soldier++;
                    turn++;
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 4) //do nothing
            {
                turn++;
            }
        }
    }
    if (winner)
        save->end = 1;
    return ((save->end && winner == root.state->turn) || (save->kingdom[1].soldier <= save->kingdom[2].soldier));
}

void backpropagation(node* node, int result) {
    while (node != NULL) {
        node->visits++;
        node->winCount += result;
        node = node->parent;
    }
}

int possibleMoves (gameState* state) {
    LoadGame(state);
    kingdoms[turn].availableNumber = 0;
    checkNeighbors(kingdoms[turn].x, kingdoms[turn].y);

    for (int i = 0; i <kingdoms[turn].roadNumber; ++i) {
        checkNeighbors(kingdoms[turn].road[i].x, kingdoms[turn].road[i].y);
    }

    for(int i=0; i<villageNumber; i++) {
        if(villages[i].kingdom ==turn) {
            checkNeighbors(villages[i].x, villages[i].y);
        }
    }
    int moveCount = kingdoms[turn].availableNumber;
    if (kingdoms[turn].foodX < 3 && kingdoms[turn].worker < 4) {
        moveCount++;
    }
    if (kingdoms[turn].worker <4 && kingdoms[turn].food >=3) {
        moveCount++;
    }
    if (kingdoms[turn].gold>=2)
        moveCount++;
    moveCount ++;
    SaveGame(state);
    return moveCount;
}

void freeTree(node *parent) {
    if (!parent) return;

    for (int i = 0; i < parent->childCount; ++i) {
        freeTree(parent->children[i]);
        parent->children[i] = NULL;
    }

    free(parent->state);
    parent->state = NULL;
    if (parent->children) {
        free(parent->children);
        parent->children = NULL;
    }
    free(parent);
    parent = NULL;
}