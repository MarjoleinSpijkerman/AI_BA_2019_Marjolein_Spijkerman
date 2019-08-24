#define RULES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fixPositions2(tree *t, int n);
void fixPositionsTableaux(tableauxTree *t, int n);

tableauxTree insertInTableauxLeavesBoth(tableauxTree ta, tree tL, tree tR, int relation, int world, int pos);
tableauxTree insertInTableauxLeaves(tableauxTree ta, tree t, int relation, int world, int pos, char r, int left, int right);

void changeWorlds(tableauxTree ta, int world, int next);

void AndPositive(tableauxTree ta, tree tL, tree tR, int world);
void AndNegative(tableauxTree ta, tree tL, tree tR, int world);
void OrNegative(tableauxTree ta, tree tL, tree tR, int world);
void OrPositive(tableauxTree ta, tree tL, tree tR, int world);

int findConnectionBeforeOnBranch(tableauxTree ta, int world, int next, int relation, tree tL);				
int findConnectionAfterOnBranch(tableauxTree ta, tableauxTree original, int world, int next, int relation, tree tL);

int countNodes(tableauxTree ta);
int checkForDuplicates(tableauxTree ta, tree t, int world, int relation);
int checkIfSameTree(tree new, tree old);
void rules(tableauxTree ta);
