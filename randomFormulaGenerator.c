#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "randomFormulaGenerator.h"

void add(char **arr, char u, int *size, int *loc){
	if ((*loc) == (*size)){
		(*size)++;
		(*arr) = realloc((*arr), (*size) * sizeof(char));
	}
	(*arr)[(*loc)] = u;
	(*loc)++;
}
	

void Identifier(char **arr, int *size, int *loc){
	//srand(time(NULL));
	int num = rand()%5;
	if (num == 0){
		add(arr, 'A', &(*size), &(*loc));
	} else if (num == 1){
		add(arr, 'C', &(*size), &(*loc));
	} else if (num ==2){
		add(arr, 'E', &(*size), &(*loc));
	} else if (num == 3){
		add(arr, 'F', &(*size), &(*loc));
	} else if (num == 4){
		add(arr, 'G', &(*size), &(*loc));
	}
}

void Symbol (char **arr, int *size, int *loc){
	//srand(time(NULL));
	int num = rand()%3;
	if (num == 0){
		add(arr, 'B', &(*size), &(*loc));
	} else if (num == 1){
		add(arr, 'D', &(*size), &(*loc));
	} else if (num ==2){
		add(arr, '~', &(*size), &(*loc));
	}
}

void Connective(char **arr, int *size, int *loc){
	//srand(time(NULL));
	int num = rand()%2;
	if (num == 0){
		add(arr, '&', &(*size), &(*loc));
	} else if (num == 1){
		add(arr, '|', &(*size), &(*loc));
	}
}

void Atom(char **arr, int *size, int *loc){
	//srand(time(NULL));
	int num = rand()%2;
	if (num == 0){
		Identifier(arr, &(*size), &(*loc));
	} else if (num == 1){
		add(arr, '(', &(*size), &(*loc));
		Expression(arr, &(*size), &(*loc));
		add(arr, ')', &(*size), &(*loc));
	}
}

void Formula(char **arr, int *size, int *loc){
	//srand(time(NULL));
	int num = rand()%3;
	
	while (num != 0){
		Symbol(arr, &(*size), &(*loc));
		num--;
	}
	Atom(arr, &(*size), &(*loc));
}

void Expression(char **arr, int *size, int *loc){
	//srand(time(NULL));
	int num = rand()%2;
	
	Formula(arr, &(*size), &(*loc));
	while (num != 0){
		Connective(arr, &(*size), &(*loc));
		Formula(arr, &(*size), &(*loc));
		num--;
	}
	return;
}

int count (char *arr, int size){
	int cnt = 0;
	int cnt2 = 0;
	for(int i = 0; i<size; i++){
		if (arr[i] != '(' && arr[i] != ')'){
			cnt++;
		}
		if (arr[i] == 'B' || arr[i] == 'D'){
			cnt2++;
		}
		if (cnt2 > 4){
			return 0;
		}
	}
	return cnt;
}
		 
int main(int argc, char *argv[]) {
	int i;
	int j;
	int numOfPremises;
	int lengthOfPremise;
	
	printf("How many premises do you want\n");
	scanf("%d", &numOfPremises);
	printf("How many symbols should each formula have\n");
	scanf("%d", &lengthOfPremise);
	
	FILE * fp1;
	fp1 = fopen ("name.txt","w");
	fprintf(fp1, "%d\n", 50);
	for(i = 0; i < 50;i++){
		fprintf(fp1, "%d\n", numOfPremises);
		j = 0;
		while(numOfPremises+1 > j){
			int loc = 0;
			int size = 1; 
			char *arr = malloc(size*sizeof(char));
			if (loc == size){
				size++;
				arr = realloc(arr, (size) * sizeof(char));
			}
			Expression(&arr, &size, &loc);
			size++;
			arr = realloc(arr, (size) * sizeof(char));
			arr[loc] = '\0';
			if (count(arr, size) == lengthOfPremise+1){
				fprintf(fp1, "%s\n", arr);
				fprintf(fp1, "y\n");
				j++;
			}
			free(arr);
		}
	}
	fclose (fp1);
	return 0;
}
