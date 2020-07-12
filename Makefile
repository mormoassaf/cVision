FILES = lib/image/image.h lib/image/image.cpp lib/image/stb_image.h lib/image/stb_image_resize.h lib/image/stb_image_write.h lib/utils.h
MAIN = main.cpp

all:
	g++ $(FILES) $(MAIN) -Wall -pedantic -o test -lm