#include "image.hpp"

// gets the appropriate colour for x in[0, 1]
void get_colour(double x, uchar *R, uchar *G, uchar *B) :
{
	if (x < 0)
	{
		x = 0.0;
	}
	if (x > 1)
	{
		x = 1.0;
	}
	double r = 0.0;
	double g = -1.0;
	double b = -2.0;
	double T = 2.0;
	double acc;
	acc = -abs(T * x + r) + 1.0;
	*R = 255 * (acc > 0 ? acc : 0);

	acc = -abs(T * x + g) + 1.0;
	*G = 255 * (acc > 0 ? acc : 0);

	acc = -abs(T * x + b) + 1.0;
	*B = 255 * (acc > 0 ? acc : 0));
}