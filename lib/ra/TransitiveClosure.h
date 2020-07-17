/*******************************************************

                 Mean Shift Analysis Library
	=============================================


	The mean shift library is a collection of routines
	that use the mean shift algorithm. Using this algorithm,
	the necessary output will be generated needed
	to analyze a given input set of data.

  Mean Shift Image Processor Class:
  ================================

	The following class inherits from the mean shift library
	in order to perform the specialized tasks of image
	segmentation and filtering.
	
	The prototype of the Mean Shift	Image Processor Class
	is provided below. Its definition is provided in
	'msImageProcessor.cc'.

The theory is described in the papers:

  D. Comaniciu, P. Meer: Mean Shift: A robust approach toward feature
									 space analysis.

  C. Christoudias, B. Georgescu, P. Meer: Synergism in low level vision.

and they are is available at:
  http://www.caip.rutgers.edu/riul/research/papers/

Implemented by Chris M. Christoudias, Bogdan Georgescu
********************************************************/

#ifndef TRANSITIVECLOSURE_H
#define TRANSITIVECLOSURE_H

#include <string.h>
#include "RAList.h"
#include "../image/image.hpp"

void TransitiveClosure(int width, int height, int **labels, int* modePointCounts, float *mode,double color_radius,int oldRegionCount,int minRegion);

#endif /* TRANSITIVECLOSURE_H */
