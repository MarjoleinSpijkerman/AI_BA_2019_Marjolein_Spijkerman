#define RANDOMFORMULAGENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add(char **arr, char u, int *size, int *loc);
	

void Identifier(char **arr, int *size, int *loc);
void Symbol (char **arr, int *size, int *loc);

void Connective(char **arr, int *size, int *loc);

void Atom(char **arr, int *size, int *loc);
void Formula(char **arr, int *size, int *loc);

void Expression(char **arr, int *size, int *loc);
