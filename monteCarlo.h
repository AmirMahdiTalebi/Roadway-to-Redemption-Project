#ifndef MAIN_C_MONTECARLO_H
#define MAIN_C_MONTECARLO_H

#include "raylib.h"
#include "War.h"
#include "InitialGame.h"
#include "GameUpdate.h"

#define CONSTANT 1.5

typedef struct node node;
typedef struct gameState gameState;
typedef struct node {
    gameState* state;
    node* parent;
    node** children;
    int childCount;
    int winCount;
    int visits;
} node;

int monte();
node* selection();
void expand(node* parent);
int simulation(gameState* state);
void backpropagation(node* node, int result);
int possibleMoves(gameState* state);
void freeTree(node *root);

#endif //MAIN_C_MONTECARLO_H
