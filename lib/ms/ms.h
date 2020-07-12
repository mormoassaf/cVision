/*
 * Copyright (c) 2019, Damir Demirović <damir.demirovic@untz.ba>
 * All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version. You should have received a copy of this license along
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MEANSHIFT_H
#define MEANSHIFT_H


#include <iostream>
#include <cmath>
#include <string.h>
#include "../image/image.h"


using namespace std;

/*Structure MSPoint define the stack */
struct MSPoint
{
    int x;
    int y;
};

uchar* MeanShift(uchar* image, uchar *filtered, int **labels, int width, int height, int spatial_radius, double color_radius, int minRegion, int num_iters);
uchar* MS_Filter(uchar* image, int width, int height, int h_spatial, double h_range, int initIters);
int MS_Segment(uchar * image, int width, int height, int **labels, double h_range, int minRegion);
int MS_Cluster(uchar  *image, int width, int height, int **labels,int* modePoints, float *mode, double h_range);


#endif /* MEANSHIFT_H */
