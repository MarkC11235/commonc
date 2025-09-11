FILE=test

all: preprocess build run

preprocess: $(FILE).mc common.h
	./preprocessor.py $(FILE).mc

build:
	gcc -o $(FILE) $(FILE).c

run:
	./$(FILE)
