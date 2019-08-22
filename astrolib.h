#ifndef ASTROLIB
#define ASTROLIB

	#include <math.h>
	#include <stdio.h>
	#include "sofa/sofa.h"
	#include "vsop87a_full.h"

	#define PI M_PI

	#define BODY_SUN 0
	#define BODY_MERCURY 1
	#define BODY_VENUS 2
	#define BODY_EARTH 3
	#define BODY_MARS 4
	#define BODY_JUPITER 5
	#define BODY_SATURN 6
	#define BODY_URANUS 7
	#define BODY_NEPTUNE 8
	#define BODY_EMB 9
	#define BODY_MOON 10

	void getBody(int bodyNum,double et, double body[3]);
	void rotvsop2J2000(double body[3]);
#endif
