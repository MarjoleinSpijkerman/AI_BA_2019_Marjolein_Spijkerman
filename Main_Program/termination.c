#include <stdio.h>
#include <stdlib.h>
#include "dataStruct.h"
#include <string.h>
#include "scanner.h"
#include "rules.h"
#include "termination.h"

/*This part of the program works as follows,
 * First the program searches for formulas containing either a single
 * atom, or a negation symbol followed by a single atom. Then using the 
 * findOpposite function, it will search for the exact same formula, 
 * but with a different relation. If this has been found, the original
 * formula, will be marked with closed. In the end, the program goes
 * through the complete tableau and will count the amount of times it
 * could reach a leaf, without coming across a "closed" mark.
 * Then it returns this sum to the main function. When the sum is 
 * bigger than 0, there are still open branches, meaning the inference
 * is invalid. If the sum is equal to 0, then the inference is valid.*/
void findOpposite(tableauxTree ta, tableauxTree original, tree t, int relation, int world){
	if (ta == NULL){
		return;
	}
	if (ta->formula != NULL){
		if (ta->formula->item >= 'A' && ta->formula->item <= 'Z' && ta->formula->item != 'D' && ta->formula->item != 'B'){
			if ((ta)->formula->item == t->item){
				if (ta->relation != relation){
					if (ta->world == world){
						original->closed = 1;
						return;
					}
				}
			}
		}
		if ((ta)->formula->item == '~' && (ta)->formula->item == t->item){
			if ((ta)->formula->left->item == t->left->item){
				if (ta->relation != relation){
					if (ta->world == world){
						original->closed = 1;
						return;
					}
				}
			}
		}
	}
	findOpposite(ta->parent, original, t, relation, world);
}

void checkForClosingBranches(tableauxTree ta){
	if (ta->formula != NULL){
		if (ta->formula->item == '~'){
			if (ta->formula->left->item >= 'A' && ta->formula->left->item <= 'Z' && ta->formula->left->item != 'D' && ta->formula->left->item != 'B'){
				tree t1 = copyTree(ta->formula);
				findOpposite(ta, ta, t1, ta->relation, ta->world);
				freeTree(t1);
			}
		}
		if (ta->formula->item >= 'A' && ta->formula->item <= 'Z' && ta->formula->item != 'D' && ta->formula->item != 'B'){
			tree t1 = copyTree(ta->formula);
			findOpposite(ta, ta, t1, ta->relation, ta->world);
			freeTree(t1);
		}
	}
}

void findUnClosedBranches(tableauxTree ta, int *sum){
	if (ta == NULL || ta->closed == 1){
		return;
	}

	if (ta->left == NULL && ta->right == NULL){
		*sum +=1;
	}
	findUnClosedBranches(ta->left, sum);
	findUnClosedBranches(ta->right, sum);
}
