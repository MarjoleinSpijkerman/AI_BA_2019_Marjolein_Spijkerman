#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStruct.h"
#include "rules.h"
#include "rulesetRegular.h"
#include "rulesetComplete.h"
#include "termination.h"

/*Ensuring the positions given to the nodes are correct, even after modifying*/
/*Used when checking how to the tableau actually looks*/
/*Usually not shown for users*/
void fixPositions2(tree *t, int n){
	if ((*t) == NULL){
		return;
	}
	(*t)->pos = n;
	fixPositions2(&(*t)->left, (n*2));
	fixPositions2(&(*t)->right, (n*2+1));
}

void fixPositionsTableaux(tableauxTree *t, int n){
	if ((*t) == NULL){
		return;
	}
	(*t)->pos = n;
	fixPositionsTableaux(&(*t)->left, (n*2));
	fixPositionsTableaux(&(*t)->right, (n*2+1));
}

/*In both functions below, when adding a new node, the program 
 * immidiately checks if the branch can be closed. If the branch can
 * be closed, this node will be marked and further new nodes cannot be 
 * added below this point. This is to ensure the branch won't go on
 * for possibly a long time after the branch already could have been closed.
 * So, it is mainly a time-saving and memory-saving mechanism.*/
/*Adds the new node, on both sides of each leaf on the branch*/
tableauxTree insertInTableauxLeavesBoth(tableauxTree ta, tree tL, tree tR, int relation, int world, int pos){
	if (ta == NULL || ta->closed == 1){
		return ta;
	}
	if (ta->left == NULL && ta->right == NULL){ /*found a leaf*/
		tree copy1 = copyTree(tL);
		tree copy2 = copyTree(tR);
		fixPositions2(&copy1, 1);
		fixPositions2(&copy2, 1);
		if (checkForDuplicates(ta, copy1, world, relation) != 1){
			ta->left = newTableauxTree(copy1, NULL, NULL, relation, world, pos*2, '0', 0, 0, ta, (ta->nextWorld));
			checkForClosingBranches(ta->left);
		}
		if (checkForDuplicates(ta, copy2, world, relation) != 1){
			ta->right = newTableauxTree(copy2, NULL, NULL, relation, world, pos*2+1, '0', 0, 0, ta, (ta->nextWorld));
			checkForClosingBranches(ta->right);
		}
		
		
	} else{
		ta->left = insertInTableauxLeavesBoth(ta->left, tL, tR, relation, world, (ta->pos)*2);
		ta->right = insertInTableauxLeavesBoth(ta->right, tL, tR, relation, world, (ta->pos)*2+1);
	}
	return ta;
}

/*Add the new nodes only to the left side of each leaf on the branch.*/
tableauxTree insertInTableauxLeaves(tableauxTree ta, tree t, int relation, int world, int pos, char r, int left, int right){
	if (ta == NULL || ta->closed == 1){
		return ta;
	}
	if (ta->left == NULL && ta->right == NULL){ /*found a leaf*/
		if (r == 'r'){
			insertInTableaux(ta, ta, NULL, relation, world, pos, r, left, right, ta->nextWorld);
		} else {
			tree copy = copyTree(t);
			fixPositions2(&copy, 1);
			if (checkForDuplicates(ta, copy, world, relation) != 1){
				insertInTableaux(ta, ta, copy, relation, world, pos, '0', 0, 0, ta->nextWorld); 
				checkForClosingBranches(ta->left);
			}
		}
	} else{
		ta->left = insertInTableauxLeaves(ta->left, t, relation, world, (ta->pos)*2, r, left, right);
		ta->right = insertInTableauxLeaves(ta->right, t, relation, world, (ta->pos)*2+1, r, left, right);
	}
	return ta;
}

/* Used to keep track of the worlds*/
void changeWorlds(tableauxTree ta, int world, int next){
	if (ta == NULL){
		return;
	}
	if (ta->nextWorld < next+1){
		ta->nextWorld = next+1;
	}
	changeWorlds(ta->left, world, next);
	changeWorlds(ta->right, world, next);
}

/*regular*/
void AndPositive(tableauxTree ta, tree tL, tree tR, int world){
	ta = insertInTableauxLeaves(ta, ta->formula->left, 1, world, (ta->pos)*2, '0', 0, 0);
	ta = insertInTableauxLeaves(ta, ta->formula->right, 1, world, (ta->pos)*2, '0', 0, 0);
} 

/*split*/
void AndNegative(tableauxTree ta, tree tL, tree tR, int world){
	ta = insertInTableauxLeavesBoth(ta, ta->formula->left, ta->formula->right, 0, world, (ta->pos));
} 

void OrNegative(tableauxTree ta, tree tL, tree tR, int world){
	ta = insertInTableauxLeaves(ta, ta->formula->left, 0, world, (ta->pos)*2, '0', 0, 0);
	ta = insertInTableauxLeaves(ta, ta->formula->right, 0, world, (ta->pos)*2, '0', 0, 0);
} 

/*split*/
void OrPositive(tableauxTree ta, tree tL, tree tR, int world){
	ta = insertInTableauxLeavesBoth(ta, ta->formula->left, ta->formula->right, 1, world, (ta->pos));
} 

/*findConnectionBeforeOnBranch and FindConnectionAfterOnBranch together work on
 * finding existing connections on the branch, both before and after the current
 * node. These are needed in order to apply the rules for Box+ and Diamond-*/
int findConnectionBeforeOnBranch(tableauxTree ta, int world, int next, int relation, tree tL){
	if (ta == NULL){
		return 1;
	}
	if (ta->r.item == 'r' && ta->r.left == world){
		insertInTableauxLeaves(ta, tL, relation, ta->r.right, ta->pos*2, '0', 0, 0);
	}
	return findConnectionBeforeOnBranch(ta->parent, world, next, relation, tL);
}
					
int findConnectionAfterOnBranch(tableauxTree ta, tableauxTree original, int world, int next, int relation, tree tL){ 
	if (ta == NULL){
		return next;
	}
	if (ta->r.item == 'r'){
		if (ta->r.right > original->checkB1D0Rule && ta->r.left == world){
			insertInTableauxLeaves(ta, tL, relation, ta->r.right, ta->pos*2, '0', 0, 0);
			original->checkB1D0Rule = ta->r.right;
		}
	}
	return findConnectionAfterOnBranch(ta->left, original, world, next, relation, tL);
	return findConnectionAfterOnBranch(ta->right, original, world, next, relation, tL);
}

/*To count the current amount of nodes in the tableau, used to keep track of newly added nodes
 *and to see if the program can stop trying to apply new nodes */
int countNodes(tableauxTree ta){
	int c = 1;
	if (ta == NULL){
		return 0;
	} else{
		c += countNodes(ta->left);
		c += countNodes(ta->right);
		return c;
	}
}

/*Check to see if the exact same formula already exists on the branch*/
int checkForDuplicates(tableauxTree ta, tree t, int world, int relation){
	if (ta == NULL){
		return 0;
	} if (world == ta->world && relation == ta->relation){
		int x = checkIfSameTree(ta->formula, t);
		if (x == 0){
			return 1;
		}
	} else{
		checkForDuplicates(ta->parent, t, world, relation);
	}
	return 0;
}

int checkIfSameTree(tree new, tree old){
	if (new == NULL || old == NULL){
		return 0;
	}
	if (new->item != old->item){
		return 1;
	}
	return checkIfSameTree(new->left, old->left);
	return checkIfSameTree(new->right, old->right);
}

/*The loop for going through the rules
 * goThroughTableauxEveryThing contains all the rules, while the numbered
 * versions only contain some of the rules. See the paper a more in depth
 * explanation. The general idea is that some rules will be applied first,
 * to avoid the branches from splitting as long as possible.*/
void rules(tableauxTree ta){
	goThroughTableaux(ta);
	fixPositionsTableaux(&ta, 1);
	
	goThroughTableaux2(ta);
	fixPositionsTableaux(&ta, 1);	
	
	goThroughTableaux3(ta, ta);
	fixPositionsTableaux(&ta, 1);
	
	goThroughTableaux4(ta, ta);
	fixPositionsTableaux(&ta, 1);
	
	goThroughTableauxEveryThing(ta, ta);
	fixPositionsTableaux(&ta, 1);

	/*For each total loop it goes through the rules it checks
	 * whether a new node has been added in the previous loop.
	 * As long as new nodes are added, these new nodes can cause
	 * that a rule can be applied over these new nodes. 
	 * When there have been no new nodes, in the previous loop,
	 * it can be assumed that everything that could been applied, has 
	 * been applied. Then this function can be stopped and the program
	 * can move to the next part.*/
	int old = 0;
	int new = countNodes(ta);
	while(new > old){
		goThroughTableauxEveryThing(ta, ta);
		fixPositionsTableaux(&ta, 1);
		old = new;
		new = countNodes(ta);
		/*These two commented-out print statements can be used if you
		 * want to keep track of the amount of nodes in the tree. 
		 * These are removed for now. But, can be umcommented when you
		 * so desire. */
		//printf("new %d\n", new);
	}
	//printf("total number of nodes in tree %d\n", new);
	return;
}
