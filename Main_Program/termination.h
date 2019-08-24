#define TERMINATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void findOpposite(tableauxTree ta, tableauxTree original, tree t, int relation, int world);
void checkForClosingBranches(tableauxTree ta);
void findUnClosedBranches(tableauxTree ta, int *sum);
