#include <stdio.h>
#include <stdlib.h>
#include "dataStruct.h"
#include <string.h>
#include "scanner.h"
#include "rules.h"
#include "termination.h"


int main(int argc, char *argv[]) {
	printf("Welcome\n");
	printf("This tableaux solver is for KFDE\n");
	printf("Please use only single character variables for the parameters\n");
	printf("These need to be capital letters, and may not include B and D\n");
	printf("The following are used for other symbols\n");
	printf("Please use D instead of \u25c7\n");  /*diamond*/
	printf("Please use B instead of \u25fb\n");  /*box*/
	printf("Please use ~ instead of \u00ac\n");  /*negation*/
	printf("please use & instead of \u2227\n");  /*and*/
	printf("please use | instead of \u2228\n");  /*or*/
	
	printf("How many inferences are you going to insert?\n");
	int num;
	scanf("%d", &num);
	
	while(num > 0){
	
		/*Scans the input*/
		tableauxTree ta = scanner();
		

		/*Applying the rules*/
		rules(ta);
		
		/*Prints the Tree, commented out for now*/
		//inOrderTableaux(ta);
		
		/*Checks for termination*/
		int x=0;
		findUnClosedBranches(ta, &x);
		if(x>0){
			printf("Not Valid\n");
		} else{
			printf("Valid\n");
		}
		/*Freeing the allocated memory*/
		freeTableauxTree(ta);
		num--;
	}
	return 0;
}
