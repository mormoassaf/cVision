FILES = lib/image/* lib/utils.h lib/ms/* lib/ra/* lib/stereo/*
MAIN = main.cpp

all:
	g++ $(FILES) $(MAIN) -Wall -pedantic -o cVision_test -lm

run:
	./cVision_test