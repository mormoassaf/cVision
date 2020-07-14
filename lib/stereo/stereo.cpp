#include "stereo.h"
#include "../image/image.hpp"
#include "../utils.h"

// new stereo object
Stereo *new_stereo(double focal_length, double baseline)
{
	Stereo *st = (Stereo *)malloc(sizeof(Stereo));
	st->left = NULL;
	st->right = NULL;
	st->dsp = NULL;
	st->d = baseline;
	st->f = focal_length;
	st->max_dsp = INT_MIN;
	st->max_z = INT_MIN;
	st->lw.i = 0;
	st->lw.j = 0;
	st->rw.i = 0;
	st->rw.j = 0;
}

// free the stereo object (this action will make it unusable)
void free_stereo(Stereo *st)
{
	if (st->dsp != NULL)
	{
		free_image_data(st->dsp);
	}
	if (st->dsp != NULL)
	{
		free_image_data(st->dsp);
	}
	free(st);
}

// set the images that need to be processed
void set_images(Stereo *st, Image *left, Image *right)
{
	st->left = left;
	st->right = right;
	ASSERT_EXIT((st->left->width == st->right->width && 
				 st->left->height == st->right->height && 
		         st->left->nchannels == st->right->nchannels), 
				"cVision/stereo: images cannot be compared!");
	// allow the same disparity array to be used multiple times
	if (st->dsp == NULL)
	{
		st->dsp = new_image_data(left->width, left->height, left->nchannels, false);
	}
	else if (!(st->dsp->width == st->left->width &&
		st->dsp->height == st->left->height &&
		st->dsp->nchannels == st->left->nchannels))
	{
		st->dsp = new_image_data(left->width, left->height, left->nchannels, false);
	}
}

// match the images before producing the final output
void match(Stereo *st, int h_spacial, int d_max)
{
	ASSERT_EXIT((st->left != NULL && st->right != NULL), "cVision/stereo: cannot compare NULL images!");
	st->lw.m = h_spacial;
	st->rw.m = h_spacial;
	double cost, min_cost;
	int disparity;

	for (int i = 0; i < st->left->width; i++)
	{
		for (int j = 0; j < st->left->height; j++)
		{
			st->lw.i = i;
			st->lw.j = j;
			min_cost = INT_MAX;
			// find best match
			for (int d = max(i - d_max, 0); d <= i; d++)
			{
				st->rw.i = d;
				st->rw.j = j;
				cost = cost(st);
				if (cost < min_cost)
				{
					min_cost = cost;
					dispartiy = abs(i - d);
				}
			}
			get_pixel(st->dsp, i, j)[0] = disparity;
		}
	}
}

// Cost function for comparing windows inside a stereo object
double cost(Stereo *st)
{
	int size = st->lw.sm * 2 + 1;
	int li, lj, ri, rj;
	double error;
	double cost = 0;
	int val1, val2;
	for (int i = 0; i <= size; i++)
	{
		for (int j = 0; j <= size; j++)
		{
			li = st->lw.i - st->lw.m + i;
			lj = st->lw.j - st->lw.m + j;
			ri = st->rw.i - st->rw.m + i;
			rj = st->rw.j - st->rw.m + j;
			for (int n = 0; n < st->left->nchannels; n++)
			{
				val1 = get_pixel(st->left, li, lj)[n];
				val2 = get_pixel(st->right, ri, rj)[n];
				error = (val1 - val2);
				error *= error;
				cost += error;
			}
		}
	}
	return cost;
}

// Get an image of the result to visualise it
Image get_result_img(Stereo *st)
{
	ImageData *data = st->dsp;
	Image *result = new_image();
	create_image(result, data->width, data->height, 3, false);

	ASSERT_EXIT((data != NULL), "cVision/stereo: the images need to be set and matched first!");
	for (int i = 0; i < data->width; i++)
	{
		for (int j = 0; j < data->height; j++)
		{
			uchar r, g, b;
			get_colour(get_pixel(data, i, j)[0]/st->max_dsp, &r, &g, &b);
			uchar *p = get_pixel(result, i, j);
			p[0] = r;
			p[1] = g;
			p[2] = b;
		}
	}
	return result;
}

