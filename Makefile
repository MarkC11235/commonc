FILE=test

main: $(FILE).mc common.h
	./preprocessor.py $(FILE).mc
	gcc -std=c99 -o $(FILE) $(FILE).c
