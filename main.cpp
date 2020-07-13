#include "cVision.hpp"

int main(int argc, char const *argv[])
{
    Image *myImg = new_image();
    load_image(myImg, "samples/bpack0.png");

    Image *result = MS_Filter(myImg, 10, 16, 5);
    Image *rgb = convertLUV2RGB(result);

    save_image(rgb, "rgb_3.png");

    // free stuff
    free_image(myImg);
    free_image(result);
    free_image(rgb);
    free(result);
    free(myImg);
    free(rgb);
    return 0;
}