#define DATASTRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*Defining the structs*/
typedef struct connectionTree{
	char item;
	int left;
	int right;
} connectionTree;

typedef struct treeNode *tree;

typedef struct treeNode {
	char item;
	tree left;
	tree right;
	int pos;
} treeNode;

typedef struct tableauxTreeNode *tableauxTree;

typedef struct tableauxTreeNode {
	tree formula;
	connectionTree r;
	tableauxTree left;
	tableauxTree right;
	tableauxTree parent;
	int relation;
	int world;
	int pos;
	int appliedRule;
	int nextWorld;
	int checkB1D0Rule; 
	int closed;
} tableauxTreeNode;


/*The functions used*/

/*creating empty trees of either type*/
tree emptyTree();
tableauxTree emptyTableauxTree();

/*creating new trees with values*/
tableauxTree newTableauxTree(tree t, tableauxTree tL, tableauxTree tR, int relation, int world, int pos, char r, int left, int right, tableauxTree parent, int next);
tree newTree(char item, tree tL, tree tR, int pos);

/*freeing the data*/
void freeTree(tree t);
void freeTableauxTree(tableauxTree t);

/*Printing the tree in 'inOrder' order*/
void inOrder(tree t);
void inOrderTableaux(tableauxTree ta);

/*helper functions for creating the trees*/
tableauxTree insertInTableaux(tableauxTree taParent, tableauxTree ta, tree t1, int relation, int world, int pos, char r, int left, int right, int next);
tree copyTree(tree t);

/*Functions for parsing the input*/
int acceptCharacter(char *arr, char c, int *pos, int len);
int treeIdentifier(char *arr, tree *t, int *pos, int len);
int treeAtom(char *arr, tree *t, int *pos, int len);
int treeFormula(char *arr, tree *t, int *pos, int len);
int treeExpression(char *arr, tree *t, int *pos, int len);
