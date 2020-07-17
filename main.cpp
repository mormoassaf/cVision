#include "cVision.hpp"

int main(int argc, char const *argv[])
{
    Image *left = new_image();
    Image *right = new_image();

    load_image(left, "samples/cone0.png");
    load_image(right, "samples/cone1.png");

    // stereo session
    printf("Starting stereo...\n");
    Stereo *st = new_stereo(10, 10);
    printf("Setting images...\n");
    set_images(st, left, right);
    printf("Matching...\n");
    match(st, 4, 100);
    printf("Getting result...\n");
    Image *result = get_result_img(st);
    printf("Freeing stereo...\n");
    free_stereo(st);
    printf("Saving result...\n");
    save_image(result, "stereo.png");

    // free stuff
    free_image(left);
    free_image(right);
    free_image(result);
    free(result);
    free(left);
    free(right);
    return 0;
}