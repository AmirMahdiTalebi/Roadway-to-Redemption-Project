#include "monteCarlo.h"
#include "Map.h"
#include "War.h"
#include "raylib.h"
#include "stdlib.h"
#include <math.h>
#include "stdio.h"
#include <limits.h>

#define constant 2
#define iterations 10

node root;

typedef struct node node;

void monte() {
    monteCarlo = 1;
    node *current = (node*) malloc(sizeof(node));
    current->state = (gameState*) malloc(sizeof(gameState));
    current->parent = NULL;
    current->children = NULL;
    SaveGame(current->state);
    current->visits=current->winCount=0;
    expand(current);
    for (int j = 0; j < current->childCount; ++j) {
        int gameResult = simulation(current->children[j]->state);
        backpropagation(current->children[j],gameResult);
    }
    root = *current;

    for (int i = 0; i < iterations ; ++i) {
        expand(current);
        for (int j = 0; j < current->childCount; ++j) {
            int gameResult = simulation(current->children[j]->state);
            backpropagation(current->children[j],gameResult);
        }
        current = selection();
    }

    node* bestMove;
    int maxSimulations = -1;
    for (int i = 0; i < root.childCount; i++) {
        if (root.children[i]->visits > maxSimulations) {
            maxSimulations = root.children[i]->visits;
            bestMove = root.children[i];
        }
    }
    LoadGame(bestMove->state);
    monteCarlo = 0;
    mode = 0;
    freeTree(&root);
}

node* selection() {
    node* best = NULL;
    double maxUCB;
    node* parent;
    parent = &root;
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
            if (ucb > maxUCB) {
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
        parent->children[i] = (node*) malloc(sizeof (node));
        parent->children[i]->state = (gameState*) malloc(sizeof (gameState));
        parent->children[i]->state = parent->state;
        parent->children[i]->parent = parent;
        parent->children[i]->children = NULL;
        parent->children[i]->childCount = 0;
        parent->children[i]->winCount = 0;
        parent->children[i]->visits = 0;

        LoadGame(parent->children[i]->state);
        int move = i+1;
        if (move <= kingdoms[turn].availableNumber) {
            roadX = kingdoms[turn].available[move].x;
            roadY = kingdoms[turn].available[move].y;
            RoadMaker();
        }
        else {
            if (move - kingdoms[turn].availableNumber ==1) { //add food
                if (kingdoms[turn].foodX < 3 && kingdoms[turn].worker < 4) {
                    action = 1;
                    mode1();
                }
                else move++;
            }
            if (move - kingdoms[turn].availableNumber == 2) { //add worker
                if (kingdoms[turn].worker <4 && kingdoms[turn].food >=3) {
                    action = 2;
                    mode1();
                }
                else move++;
            }
            if (move - kingdoms[turn].availableNumber == 3) { //add soldier
                if (kingdoms[turn].gold >=2) {
                    action = 3;
                    mode1();
                }
                else move++;
            }
            if (move - kingdoms[turn].availableNumber == 4) { //do nothing
                turn++;
            }
        }
        if (turn > kingdomNumber) {
            turn = 1;
            for (int k = 1; k <= kingdomNumber; k++) {
                if (kingdoms[k].dead)
                    continue;
                kingdoms[k].food += kingdoms[k].foodX;
                kingdoms[k].gold += kingdoms[k].goldX;
            }
        }
        SaveGame(parent->children[i]->state);
    }
}

int simulation(gameState* state) {
    LoadGame(state);
    gameState save = *state;
    while (!winner) {
        if (turn > kingdomNumber) {
            turn = 1;
            for (int i = 1; i <= kingdomNumber; i++) {
                kingdoms[i].food += kingdoms[i].foodX;
                kingdoms[i].gold += kingdoms[i].goldX;
            }
        }
        SaveGame(state);
        int move = GetRandomValue(1, possibleMoves(state));

        if (move <= kingdoms[turn].availableNumber) {
            roadX = kingdoms[turn].available[move - 1].x;
            roadY = kingdoms[turn].available[move - 1].y;
            RoadMaker();

        } else {
            if (move - kingdoms[turn].availableNumber == 1) { //add food
                if (kingdoms[turn].foodX < 3 && kingdoms[turn].worker < 4) {
                    action = 1;
                    mode1();
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 2) { //add worker
                if (kingdoms[turn].worker < 4 && kingdoms[turn].food >= 3) {
                    action = 2;
                    mode1();
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 3) {
                if (kingdoms[turn].gold >= 2) { //add soldier
                    action = 3;
                    mode1();
                } else move++;
            }
            if (move - kingdoms[turn].availableNumber == 4) //do nothing
            {
                turn++;
            }
        }
    }
    int result = (winner == 2);
    *state = save;
    return result;
}

void backpropagation(node* node, int result) {
    while (node) {
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
    moveCount += 2;
    SaveGame(state);
    return moveCount;
}

void freeTree(node *root) {
    if (!root) return;

    for (int i = 0; i < root->childCount; ++i) {
        freeTree(root->children[i]);
    }

    free(root->state);
    free(root->children);
    free(root);
}