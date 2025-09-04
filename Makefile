FILE=test

main: $(FILE).mc common.h
	./preprocessor.py $(FILE).mc
	gcc -o $(FILE) $(FILE).c
