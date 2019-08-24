#include <stdio.h>
#include <stdlib.h>
#include "dataStruct.h"
#include <string.h>
#include "scanner.h"
#include "rules.h"
#include "termination.h"

void fixPositions(tree *t, int n){
	if ((*t) == NULL){
		return;
	}
	(*t)->pos = n;
	fixPositions(&(*t)->left, (n*2));
	fixPositions(&(*t)->right, (n*2)+1);
}

char * createArray (){
	char c = ' ';
	int loc = 0;
	int size = 1; 
	char *arr = malloc(size*sizeof(char));
	while (c != '\n'){
		scanf("%c", &c);
		if (c != '\n' && c != ' '){
			if (loc == size){
				size++;
				arr = realloc(arr, (size) * sizeof(char));
			}
			arr[loc] = c;
			loc++;
		}
	}
	//ensure the array ends with an '\0', otherwise the strlen function will not properly work later ons
	size++;
	arr = realloc(arr, (size) * sizeof(char));
	arr[loc] = '\0';
	printf("Your formula was: %s\n", arr);
	return arr;
}

tableauxTree scanAndCreateTree(tableauxTree *ta, int num){
	int cnt = 1;
	while (num > 0){
		printf("Premise %d: ", cnt);
		char *arr = createArray();
		printf("Is this Correct? [y]/[n]\n");
		char z = getchar();
		getchar();
		if (z == 'n'){
		} else if (z == 'y'){
			tree t = emptyTree();
			int len = strlen(arr);
			int pos = 0;
			treeExpression(arr, &t, &pos, len);
			fixPositions(&t->left, 2);
			fixPositions(&t->right, 3);
			*ta = insertInTableaux(*ta, *ta, copyTree(t), 1, 0, 1, '0', 0, 0, 1);
			cnt++;
			num--;
			freeTree(t);
		}
		free(arr);
	}
	return (*ta);
}	

int numberOfPremises(){
	int x = -1;
	scanf("%d", &x);
	while (x < 0){
		printf("Please, fill in a number that's zero or higher\n");
		printf("What are the number of premises?\nPremises: ");
		scanf("%*s");
		scanf("%d", &x);
	}
	if (x >= 0){
		return x;
	}
	return -20;
}
	
tableauxTree scanner(){
	tableauxTree ta = emptyTableauxTree();
	printf("How many premises are you going to insert?\nPremises: ");
	int numOfPremises = 0;
	numOfPremises = numberOfPremises();
	getchar();
	printf("Thank you, now please type in every premise, followed by a newline\n");
	scanAndCreateTree(&ta, numOfPremises);
	printf("Please type in your conclusion now\n");
	printf("Conclusion: ");
	char *arr = createArray();
	
	printf("Is this Correct? [y]/[n]\n");
	char z = getchar();
	getchar();
	while (z == 'n'){
		printf("Please type in your conclusion now\n");
		printf("Conclusion: ");
		free(arr);
		char *arr = createArray();
		printf("Is this Correct? [y]/[n]\n");
		char z = getchar();
		if (z == 'y'){
			tree t = emptyTree();
			int len = strlen(arr);
			int pos = 0;
			treeExpression(arr, &t, &pos, len);
			fixPositions(&t, 1);
			ta = insertInTableaux(ta, ta, copyTree(t), 0, 0, 1, '0', 0, 0, 1);
			checkForClosingBranches(ta->left);
			freeTree(t);
			free(arr);
			return ta;
		}
		getchar();
	} if (z == 'y'){
		tree t = emptyTree();
		int len = strlen(arr);
		int pos = 0;
		treeExpression(arr, &t, &pos, len);
		fixPositions(&t, 1);
		ta = insertInTableaux(ta, ta, copyTree(t), 0, 0, 1, '0', 0, 0, 1);
		checkForClosingBranches(ta->left);
		free(arr);
		freeTree(t);
	}
	return ta;
}

	
