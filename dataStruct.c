#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dataStruct.h"
#include "rules.h"

/*Creates empty trees, by returning NULL*/
tree emptyTree(){
	return NULL;
}

tableauxTree emptyTableauxTree(){
	return NULL;
}

/*Creating the TableauxTree
 * the formula, tL, tR, relation, world and pos need to be given when creating
 * If not adding a formula, a relation irj will be kept track of in this specific node
 * appliedRule, nextWorld, and other checks are set on initial values*/
tableauxTree newTableauxTree(tree t, tableauxTree tL, tableauxTree tR, int relation, int world, int pos, char r, int left, int right, tableauxTree parent, int next){
	tableauxTree new = malloc(sizeof(struct tableauxTreeNode));
	assert(new != NULL);
	if (t == NULL){
		new->r.item = 'r';
		new->r.left = left;
		new->r.right = right;
		new->formula = NULL;
	} else{
		new->r.item = '0';
	}
	if (t!= NULL){
		new->formula = t;
	}
	new->relation = relation;
	new->world = world;
	new->left = tL;
	new->right = tR;
	new->parent = parent;
	new->pos = pos;
	new->appliedRule = 0;
	new->nextWorld = next; 
	new->checkB1D0Rule = 0;
	new->closed = 0;
	return new;
}

/*creating the tree that gives each formula in prefix notation
 * using prefix notation makes it easier to apply the rules
 * all values in the tree need to be given by creation*/
tree newTree(char item, tree tL, tree tR, int pos){
	tree new = malloc(sizeof(struct treeNode));
	assert(new != NULL);
	new->item = item;
	new->left = tL;
	new->right = tR;
	new->pos = pos;
	return new;
}

/*The two freeing functions
 * They use postOrder for traversing the tree
 * it first goes to the leftmost lowest node in the tree, 
 * frees the formula in that tree (this formula is also freed postOrder
 * then moves its way up. */
void freeTree(tree t){
	if (t == NULL){
		return;
	}
	freeTree(t->left);
	freeTree(t->right);
	free(t);
}

void freeTableauxTree(tableauxTree t){
	if (t == NULL){
		return;
	}
	freeTableauxTree(t->left);
	freeTableauxTree(t->right);
	freeTree(t->formula);
	free(t);
}

/*printing the tree using inOrder.
 * as I have not found a great way to print the tree,
 * pos numbers are also printed. These can then be written down on paper,
 * as to check for the correct order. These are mainly helper functions for
 * me as a programmer, or for users who want a general idea of how the tableaux looks.
 * the position numbers are given as follows:
 * The root = 1
 * left child = parent*2
 * right child = parent*2+1
 * 
 * this gives the following structure:
 * 			1
 * 		2		3
 * 	 3    4   5    6
 * etc. 	
 * */
void inOrder(tree t){
	if (t == NULL){
		return;
	}
	inOrder(t->left);
	printf("%c %d ", t->item, t->pos);
	inOrder(t->right);
}

void inOrderTableaux(tableauxTree ta){
	if (ta == NULL){
		return;
	}
	inOrderTableaux(ta->left);
	if (ta->formula != NULL){
		inOrder(ta->formula);
	} else {
		printf("%d %c %d ", ta->r.left, ta->r.item, ta->r.right);
	}
	if (ta->relation == 1){
		printf("+ ");
	} else printf("- ");
	printf("%d %d", ta->world, ta->pos);
	printf("\n");
	inOrderTableaux(ta->right);
}

/*This function is used to insert each scanned formula in the tableaux
 * It will insert in the first free left position on the branch.
 * More complex versions of this function are found later in the code,
 * where they will be used to insert formulas on each leaf.*/
tableauxTree insertInTableaux(tableauxTree ta, tableauxTree taParent, tree t1, int relation, int world, int pos, char r, int left, int right, int next){
	if (ta == NULL){
		tableauxTree ta1 = emptyTableauxTree();
		if (t1 == NULL){
		}
		return newTableauxTree(t1, ta1, ta1, relation, world, pos, r, left, right, taParent, next);
	} else{
		taParent = ta;
		ta->left = insertInTableaux(ta->left, taParent, t1, relation, world, (pos*2), r, left, right, next);
	}
	return ta;
}

/*Used to make copies of an existing formula, 
 * to avoid changing the original, while still being 
 * able to work with the contents of the original.*/
tree copyTree(tree t){
	tree t1 = NULL;
	if (t!=NULL){
		t1 = malloc(sizeof(treeNode));
		assert (t1!=NULL);
		t1->item = t->item;
		t1->pos = t->pos;
		t1->left = copyTree(t->left);
		t1->right = copyTree(t->right);
	}
	return t1;
}


/*These functions work together to change a scanned array that contains a formula
 * into the prefix tree structure.
 * It works according to a grammar, to ensure the correct items go to the correct 
 * places in the tree*/
 
/*check if arr[pos] is certain character*/
int acceptCharacter(char *arr, char c, int *pos, int len){
	if (arr[(*pos)] == c){
		return 1;
	}
	return 0;
}
/*identifier = 'A' | 'B' | .. | 'Z'
 *Excluding B and D*/
int treeIdentifier(char *arr, tree *t, int *pos, int len) {
	if (arr[(*pos)] >= 'A' && arr[(*pos)] <= 'Z' && arr[(*pos)] != 'D' && arr[(*pos)] != 'B'){
		*t = newTree(arr[(*pos)], NULL, NULL, 1);
		if ((*pos)+1 < len){
			(*pos)++;
		}
		return 1;
	}
	return 0;
}
// <atom>  ::=  <identifier> | '(' <expression> ')'
int treeAtom(char *arr, tree *t, int *pos, int len) {
	if (treeIdentifier(arr, t, &(*pos), len)) {
		return 1;
	}
	if (acceptCharacter(arr,'(', &(*pos), len)){
		(*pos)++;
		if (treeExpression(arr, t, &(*pos), len)){
			if  (acceptCharacter(arr,')', &(*pos), len)){
				(*pos)++;
				return 1;
			}
		}
	}
	return 0;
}

int treeFormula(char *arr, tree *t, int *pos, int len) {
	if (treeAtom(arr, t, &(*pos), len)){
		return 1;
	}
	int cnt = 0;
	int lastPos = 0;
	while (acceptCharacter(arr, '~', &(*pos), len) || acceptCharacter(arr, 'B', &(*pos), len) || acceptCharacter(arr, 'D', &(*pos), len)){
		lastPos = (*pos);
		cnt++;
		(*pos)++;
	}
	tree tL = NULL;
	if (treeAtom(arr, &tL, &(*pos), len)){
		(*t) = newTree(arr[lastPos], tL, NULL, 1);
		cnt--;
		lastPos--;
	}
	while (cnt != 0){
		tL = (*t);
		(*t) = newTree(arr[lastPos], tL, NULL, 1);
		cnt--;
		lastPos--;
	}
	return 1;
}
		
int treeExpression(char *arr, tree *t, int *pos, int len){
	if (!(treeFormula(arr, t, &(*pos), len))){
		return 0;
	}
	while(acceptCharacter(arr, '&', &(*pos), len) || acceptCharacter(arr, '|', &(*pos), len)){
		int extraPos = (*pos);
		(*pos)++;
		tree tL = *t;
		tree tR = NULL;
		if (!(treeFormula(arr, &tR, &(*pos), len))){
			freeTree(tR);
			return 0;
		}
		*t = newTree(arr[extraPos], tL, tR, 1);
	}
	return 1;
}

