#include "cVision.hpp"

int main(int argc, char const *argv[])
{
    Image *left = new_image();
    Image *right = new_image();
    
    load_image(left, "samples/chair0.png");
    load_image(right, "samples/chair1.png");

    Image *left_resized = resize_image(left, 768, 540);
    Image *right_resized = resize_image(right, 768, 540);

    // stereo session
    printf("Starting stereo...\n");
    Stereo *st = new_stereo(10, 10);
    printf("Setting images...\n");
    set_images(st, left_resized, right_resized);
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

    free_image(left_resized);
    free_image(right_resized);

    free_image(result);
    free(left);
    free(right);
    free(left_resized);
    free(right_resized);

    free(result);
    return 0;
}