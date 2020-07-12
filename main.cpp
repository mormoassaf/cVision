#include "cVision.hpp"
#include "lib/image/image.h"    

int main(int argc, char const *argv[])
{
    Image *myImg = new_image();
    load_image(myImg, "samples/bpack0.png");
    save_image(myImg, "test.png");
    free_image(myImg);
    free(myImg);
    return 0;
}