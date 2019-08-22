#include <math.h>
#include <stdio.h>
#include "sofa/sofa.h"
#include "vsop87a_full.h"

void rotvsop2J2000_1(double body[3]);
#define PI M_PI

void simpleReduction(){
	//convert Date to utc
	double utc1,utc2;
	iauDtf2d("UTC",2000,1,1,12,0,0,&utc1,&utc2);

	//convert UTC to TAI
	double atomic1,atomic2;
	iauUtctai(utc1,utc2,&atomic1,&atomic2);

	//convert TAI to TT
	double tt1,tt2;
	iauTaitt(atomic1,atomic2,&tt1,&tt2);

	double tt=tt1+tt2;
	double et=(tt-2451545.0)/365250.0;
	double earth[3];
	double emb[3];
	double moon[3];
	vsop87a_full_getEarth(et,earth);
	vsop87a_full_getEmb(et,emb);
	vsop87a_full_getMoon(earth,emb,moon);
	moon[0]-=earth[0];
	moon[1]-=earth[1];
	moon[2]-=earth[2];

	double distance = sqrt(moon[0] * moon[0] + moon[1] * moon[1] + moon[2] * moon[2]);
	distance*=1.496e+11; //Convert from AU to meters
	double lightTime=distance/299792458.0;
	et-=lightTime / 24.0 / 60.0 / 60.0 / 365250.0;

	vsop87a_full_getEarth(et,earth);
	vsop87a_full_getEmb(et,emb);
	vsop87a_full_getMoon(earth,emb,moon);
	moon[0]-=earth[0];
	moon[1]-=earth[1];
	moon[2]-=earth[2];

	rotvsop2J2000_1(moon);

	//Use UT1 for Earth Rotation Angle
	double era=iauEra00(utc1,utc2);

	double lat=38.2464000*PI/180.0;
	double lon=274.236400*PI/180.0;
	double observerPV[2][3];
	iauPvtob(lon,lat,0,0,0,0,era,observerPV);

	observerPV[0][0]/=1.49597870691E+11;
	observerPV[0][1]/=1.49597870691E+11;
	observerPV[0][2]/=1.49597870691E+11;

	/*
	printf("Observer Position and Velocity:\r\n%2.10f %2.10f %2.10f\r\n%2.10f %2.10f %2.10f\r\n\r\n",
			observerPV[0][0],observerPV[0][1],observerPV[0][2],
			observerPV[1][0],observerPV[1][1],observerPV[1][2]
			);
	*/

	moon[0]-=observerPV[0][0];
	moon[1]-=observerPV[0][1];
	moon[2]-=observerPV[0][2];

	double r = sqrt(moon[0] * moon[0] + moon[1] * moon[1] + moon[2] * moon[2]);
	double dec = acos(moon[2] / r);
	double ra = atan2(moon[1], moon[0]);

	if(ra<0) ra+=2*PI;
	if(dec<0) dec+=2*PI;
	dec=.5*PI-dec;

	printf("Moon: %f %f\r\n",ra*180.0/PI,dec*180.0/PI);
	printf("Diff: %f %f\r\n",ra*180.0/PI-222.80147,dec*180.0/PI-(-11.57774));

	//Convert to altaz
	double GMST=iauGmst06(utc1,utc2,tt1,tt2);

	double h=GMST + lon - ra;

	double sina=sin(dec)*sin(lat)+cos(dec)*cos(h)*cos(lat);
	double a=asin(sina);

	double cosAz=(sin(dec)*cos(lat)-cos(dec)*cos(h)*sin(lat))/cos(a);
	double Az=acos(cosAz);

	if(sin(h)>0){Az=2.0*PI-Az;}

	double alt=a;
	double az=Az;

	printf("Alt Az: %f %f\r\n",alt*180.0/PI,az*180.0/PI);
	printf("Diff  : %f %f\r\n",alt*180.0/PI-33.6752,az*180.0/PI-146.3255);


}

void rotvsop2J2000_1(double body[3]){
	double a,b,c;
	a = body[0] + body[1] * 0.000000440360 + body[2] * -0.000000190919;
	b = body[0] * -0.000000479966 + body[1] * 0.917482137087 + body[2] * -0.397776982902;
	c = body[1] * 0.397776982902 + body[2] * 0.917482137087;

	body[0]=a;
	body[1]=b;
	body[2]=c;

}
