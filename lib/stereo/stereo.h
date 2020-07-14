/*  ________________________________________________  *
 * |                                                | *
 * |       / University of Groningen        \       | *
 * |       \ Moe Assaf                      /       | *
 * |       / cVision/stereo                 \       | *
 * |       \ Monday the 6th of July, 2020   /       | *
 * |________________________________________________| *
 *                                                   */

#ifndef STEREO_H
#define STEREO_H

#include "../image/image.hpp"
#include <limits.h>


/*
 * How to use:
 * 1. Create a new stereo object using new_stereo() and set the specs of the camera
 * 2. Set the images that need to be processed
 * 3. Run match() to run the algorithm
 * 4. Retrieve output image or the depth map using get_depth_map() or get_result_img()
 * 5. Repeat step 2-4 optionally to process other images
 * 6. Call free_stereo() to free the memory allocate
*/

typedef struct
{
	int i;
	int j;
	int m;
} Window;

typedef struct
{
	Image *left, *right; //left and right images
	Window lw, rw;
	ImageData* dsp; // disparities
	double f; // focal length
	double d; // baseline length
	double max_z;
	int max_dsp;
} Stereo;

// Basic functionalities
Stereo *new_stereo(double focal_length, double baseline);
void free_stereo(Stereo *st);
void set_images(Stereo *st, Image *left, Image *right);
void match(Stereo *st, int h_spacial, int d_max);
Image get_result_img(Stereo *st);
double cost(Stereo *st);



#endif