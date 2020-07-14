/*
 * Copyright (c) 2018, Damir Demirović <damir.demirovic@untz.ba>
 * All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version. You should have received a copy of this license along
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGE_H
#define IMAGE_H


#include <cmath>
#include <vector>
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "../utils.h"

using namespace std;

typedef unsigned char uchar;

enum allocation_type {
    NO_ALLOCATION, SELF_ALLOCATED, STB_ALLOCATED
};

typedef struct {
    int width;
    int height;
    int nchannels;
    size_t size;
    uchar *data;
    enum allocation_type allocation_;
} Image;

Image *new_image();
void load_image(Image *img, const char *fname);
void create_image(Image *img, int width, int height, int nchannels, bool zeroed);
void save_image(const Image *img, const char *fname);
void free_image(Image *img);


// processing

uchar *get_pixel(Image *img, int x, int y);
void set_pixel(Image *img, int x, int y, const uchar val, int channel);
int** generate_labels(size_t width, size_t height);
void label_image(Image *res, int** labels, int regCount);
int range_distance(Image *img, int x1, int y1, int x2, int y2);
Image *convertRGB2LUV(Image *input);
Image *convertLUV2RGB(Image *origin);
float color_distance(const float* a, const float* b);
std::vector<int> get_random_numbers(int num);


#endif /* IMAGE_H */

