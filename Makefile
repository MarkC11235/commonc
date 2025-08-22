FILE=test

main: $(FILE).c common.h
	gcc -std=c99 -o $(FILE) $(FILE).c
