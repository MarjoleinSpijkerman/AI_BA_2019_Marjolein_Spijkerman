#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStruct.h"
#include "rules.h"
#include "rulesetRegular.h"
#include "rulesetComplete.h"

/*Ruleset for non modal and non branch-splitting rules.
 * Also the rules for double negation and ~(A&C) and ~(A|C)
 * ta->appliedRule keeps track of a certain rule has already been applied*/
void goThroughTableaux(tableauxTree ta){
	if (ta == NULL){
		return;
	}
	if (ta->appliedRule == 0){
		char c = (ta)->formula->item;
		if (c == '&'){
			if ((ta)->relation == 1){
				AndPositive(ta, NULL, NULL, ta->world);
				ta->appliedRule = 1;
			} else{
				//Do nothing in this case.
			}
		}
		if (c == '|'){
			if ((ta)->relation == 1){
				//Do nothing in this case.
			} else{
				OrNegative(ta, NULL, NULL, ta->world);
				ta->appliedRule = 1;
			}
		}
		
		if (c == '~'){
			if ((((ta)->formula)->left)->item == '&'){
				tree tL = copyTree(ta->formula->left->left);
				tree tR = copyTree(ta->formula->left->right);
				tree tLL = newTree('~', tL, NULL, 1);
				tree tRR = newTree('~', tR, NULL, 1);
				tree copy = newTree('|', tLL, tRR, 1);
				fixPositions2(&(copy), 1);
				insertInTableauxLeaves(ta, copy, ta->relation, ta->world, (ta)->pos, '0', 0, 0);
				freeTree(copy);
				ta->appliedRule = 1;	
			}
			if ((((ta)->formula)->left)->item == '|'){
				tree tL = copyTree(ta->formula->left->left);
				tree tR = copyTree(ta->formula->left->right);
				tree tLL = newTree('~', tL, NULL, 1);
				tree tRR = newTree('~', tR, NULL, 1);
				tree copy = newTree('&', tLL, tRR, 1);
				fixPositions2(&(copy), 1);
				insertInTableauxLeaves(ta, copy, ta->relation, ta->world, (ta)->pos, '0', 0, 0);	
				freeTree(copy);
				ta->appliedRule = 1;
			}
			if ((((ta)->formula)->left)->item == '~'){
				tree tL = copyTree(ta->formula->left->left->left);
				tree tR = copyTree(ta->formula->left->left->right);
				char x = (ta)->formula->left->left->item;
				tree copy = newTree(x, tL, tR, 1);
				fixPositions2(&(copy), 1);
				insertInTableauxLeaves(ta, copy, ta->relation, ta->world, (ta)->pos, '0', 0, 0);
				freeTree(copy);
				ta->appliedRule = 1;
			}
		}
	}
	goThroughTableaux(ta->left);
	goThroughTableaux(ta->right);
}

/*Non modal rules that split the branch.*/
void goThroughTableaux2(tableauxTree ta){
	if (ta == NULL){
		return;
	}
	if (ta->appliedRule == 0){
		char c = (ta)->formula->item;
		if (c == '&'){
			if ((ta)->relation == 0){
				/*split*/
				(ta)->appliedRule = 1;
				AndNegative(ta, NULL, NULL, ta->world);
			}
		}
		if (c == '|'){
			if ((ta)->relation == 1){
				/*split*/
				(ta)->appliedRule = 1;
				OrPositive(ta, NULL, NULL, ta->world);
			}
		}
	}
	goThroughTableaux2(ta->left);
	goThroughTableaux2(ta->right);
}

/*modal rules
 * 
 * 
 * 
 * */
void goThroughTableaux3(tableauxTree ta, tableauxTree beginning){
	if (ta == NULL){
		return;
	}
	if (ta->formula != NULL){
		char c = (ta)->formula->item;
		if (c == '~'){
			if (ta->appliedRule == 0){
				if ((ta)->formula->left->item == 'B'){
					tree tL = copyTree((ta)->formula);
					tL->item = 'D';
					tL->left->item = '~';
					fixPositions2(&tL, 1);
					insertInTableauxLeaves(ta, tL, ta->relation, ta->world, (ta)->pos*2, '0', 0, 0);
					freeTree(tL);
					ta->appliedRule = 1;
				}
				if ((ta)->formula->left->item == 'D'){
					tree tL = copyTree((ta)->formula);
					tL->item = 'B';
					tL->left->item = '~';
					fixPositions2(&tL, 1);
					insertInTableauxLeaves(ta, tL, ta->relation, ta->world, (ta)->pos*2, '0', 0, 0);
					freeTree(tL);
					ta->appliedRule = 1;
				}
			}
		}
		if (c == 'B'){
			if (ta->appliedRule == 0){
				if ((ta)->relation == 1){
					
				}
				if ((ta)->relation == 0){
					int world = (ta->world);
					int next = (ta)->nextWorld;
					insertInTableauxLeaves(ta, ta->formula->left, (ta)->relation, next, ((ta)->pos)*2, '0', 0, 0);
					insertInTableauxLeaves(ta, NULL, ta->relation, ta->world, (ta)->pos*2, 'r', world, next);
					changeWorlds(ta, world, next);
					ta->appliedRule = 1;
				}
			}
		}
		if (c == 'D'){
			if (ta->appliedRule == 0){
				if ((ta)->relation == 1){
					int world = (ta->world);
					int next = (ta)->nextWorld;
					insertInTableauxLeaves(ta, ta->formula->left, (ta)->relation, next, ((ta)->pos)*2, '0', 0, 0);
					insertInTableauxLeaves(ta, NULL, ta->relation, ta->world, (ta)->pos*2, 'r', world, next);
					changeWorlds(ta, world, next);
					ta->appliedRule = 1;
				}
				if ((ta)->relation == 0){
				}
			}
		}
	}
	goThroughTableaux3(ta->left, ta);
	goThroughTableaux3(ta->right, ta);
}

void goThroughTableaux4(tableauxTree ta, tableauxTree beginning){
	if (ta == NULL){
		return;
	}
	if (ta->formula != NULL){
		char c = (ta)->formula->item;
		if (c == 'B'){
			if ((ta)->relation == 1){
				int world;
				int next;
				if (ta->checkB1D0Rule == 0){
					world = ta->world;
					next = ta->nextWorld;
					findConnectionBeforeOnBranch(ta->parent, world, next, ta->relation, ta->formula->left);
					ta->checkB1D0Rule = -1;
				}
				world = ta->world;
				next = ta->nextWorld;
				findConnectionAfterOnBranch(ta, ta, world, next, ta->relation, ta->formula->left);
			}
			if ((ta)->relation == 0){
				//Do nothing here
			}
		}
		if (c == 'D'){
			if ((ta)->relation == 1){
				//Do nothing here
			}
			if ((ta)->relation == 0){
				int world;
				int next;
				int relation;
				if (ta->checkB1D0Rule == 0){
					world = ta->world;
					next = ta->nextWorld;
					relation = ta->relation;
					findConnectionBeforeOnBranch(ta->parent, world, next, relation, ta->formula->left);
					ta->checkB1D0Rule = -1;
				}
				world = ta->world;
				next = ta->nextWorld;
				relation = ta->relation;
				findConnectionAfterOnBranch(ta, ta, world, next, relation, ta->formula->left);
			}
		}	
	}
	goThroughTableaux4(ta->left, ta);
	goThroughTableaux4(ta->right, ta);
}
