CC     = gcc
CFLAGS = -g -std=c99 -pedantic -Wall

tree: main.c dataStruct.c scanner.c rules.c termination.c rulesetComplete.c rulesetRegular.c
	$(CC) $(CFLAGS) $^ -o $@
	
.PHONY: run example1 clean

run: tree
	./tree
	 
example1: tree
	cat InputScript.txt | ./tree

example2: tree
	cat TestSet.txt | ./tree

clean:
	rm -f tree
