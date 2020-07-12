#include "cVision.hpp"
#include "lib/image/image.h"    

int main(int argc, char const *argv[])
{
    Image *myImg = new_image();
    load_image(myImg, "samples/bpack0.png");

    Image *result = new_image();
    result->data = MS_Filter(myImg->data, myImg->width, myImg->height, 4, 16, 4);
    result->width = myImg->width;
    result->height = myImg->height;
    result->nchannels = 3;
    result->size = result->height * result->width * 3;
    result->data = ConvertLUV2RGB(result->data, result->width, result->height, result->nchannels);

    save_image(result, "result.png");
    // free stuff
    free_image(myImg);
    free_image(result);
    free(result);
    free(myImg);
    return 0;
}