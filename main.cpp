#include "cVision.hpp"
#include "lib/image/image.h"
#include "lib/image/stb_image.h"


int main(int argc, char const *argv[])
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load("path_to_raster_image.jpg",
                                    &width,
                                    &height,
                                    &channels,
                                    STBI_rgb);
    stbi_image_free(image);
    return 0;
}