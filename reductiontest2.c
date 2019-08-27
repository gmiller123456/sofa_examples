#include <math.h>
#include <stdio.h>
#include "sofa/sofa.h"
#include "vsop87a_full.h"
#include "astrolib.h"

void reductionTest(double utc1, double utc2, int bodyNum,
		double expectedRA, double expectedDec, double expectedAz, double expectedAlt);

void simpleReduction2(){
	//convert Date to utc
	double utc1,utc2;

	//[new Date(Date.UTC(2000,0, 1,12,0,0,0)),astrolib.MOON,38.2464000,274.236400,222.80147, -11.57774, 146.3255,  33.6752],
	iauDtf2d("UTC",2000,1,1,12,0,0,&utc1,&utc2);
	reductionTest(utc1,utc2, BODY_MOON,222.80147, -11.57774,146.3255,  33.6752);

	//[new Date(Date.UTC(2020,3,11,16,0,0,0)),astrolib.MOON,38.2464000,274.236400,249.19588, -20.90403, 261.7692, -24.3259],
	iauDtf2d("UTC",2020,4,11,16,0,0,&utc1,&utc2);
	reductionTest(utc1,utc2, BODY_MOON,249.19588, -20.90403, 261.7692, -24.3259);
}

void reductionTest(double utc1, double utc2, int bodyNum,
		double expectedRA, double expectedDec, double expectedAz, double expectedAlt){
	//convert UTC to TAI
	double atomic1,atomic2;
	iauUtctai(utc1,utc2,&atomic1,&atomic2);

	//convert TAI to TT
	double tt1,tt2;
	iauTaitt(atomic1,atomic2,&tt1,&tt2);

	double et=(tt1-2451545.0+tt2)/365250.0;

	//Compute initial position
	double earth[3];
	double body[3];

	getBody(BODY_EARTH,et,earth);
	//vsop87a_full_getEarth(et,earth);

	getBody(bodyNum,et,body);
	body[0]-=earth[0];
	body[1]-=earth[1];
	body[2]-=earth[2];

	//Compute light time to body, then recompute for apparent position
	double distance = sqrt(body[0] * body[0] + body[1] * body[1] + body[2] * body[2]);
	distance*=1.496e+11; //Convert from AU to meters
	double lightTime=distance/299792458.0;
	et-=lightTime / 24.0 / 60.0 / 60.0 / 365250.0;

	getBody(bodyNum,et,body);
	body[0]-=earth[0];
	body[1]-=earth[1];
	body[2]-=earth[2];

	//Convert VSOP87 coordinates to J2000
	rotvsop2J2000(body);

	//Get the precession, nutation, and bias matrix
	double rnpb[3][3];
	iauPnm06a(tt1, tt2, rnpb);
	//printf("NBP Matrix:\r\n%2.10f %2.10f %2.10f\r\n%2.10f %2.10f %2.10f\r\n%2.10f %2.10f %2.10f\r\n\r\n",
	//		rnpb[0][0],rnpb[0][1],rnpb[0][2],
	//		rnpb[1][0],rnpb[1][1],rnpb[1][2],
	//		rnpb[2][0],rnpb[2][1],rnpb[2][2]
	//		);

	//double x=body[0]*rnpb[0][0]+body[1]*rnpb[1][0]+body[2]*rnpb[2][0];
	//double y=body[0]*rnpb[0][1]+body[1]*rnpb[1][1]+body[2]*rnpb[2][1];
	//double z=body[0]*rnpb[0][2]+body[1]*rnpb[1][2]+body[2]*rnpb[2][2];
	//double x=body[0]*rnpb[0][0]+body[1]*rnpb[0][1]+body[2]*rnpb[0][2];
	//double y=body[0]*rnpb[1][0]+body[1]*rnpb[1][1]+body[2]*rnpb[1][2];
	//double z=body[0]*rnpb[2][0]+body[1]*rnpb[2][1]+body[2]*rnpb[2][2];

	//body[0]=x;
	//body[1]=y;
	//body[2]=z;

	iauRxp(rnpb,body,body);

	//Use UT1 for Earth Rotation Angle
	double era=iauEra00(utc1,utc2);

	//Get observer's xyz coordinates in J2000 coords
	double lat=38.2464000*PI/180.0;
	double lon=274.236400*PI/180.0;
	double observerPV[2][3];
	iauPvtob(lon,lat,0,0,0,0,era,observerPV);

	iauTr(rnpb,rnpb);
	iauRxpv(rnpb,observerPV,observerPV);

	observerPV[0][0]/=1.49597870691E+11;
	observerPV[0][1]/=1.49597870691E+11;
	observerPV[0][2]/=1.49597870691E+11;

	/*
	printf("Observer Position and Velocity:\r\n%2.10f %2.10f %2.10f\r\n%2.10f %2.10f %2.10f\r\n\r\n",
			observerPV[0][0],observerPV[0][1],observerPV[0][2],
			observerPV[1][0],observerPV[1][1],observerPV[1][2]
			);
	*/

	//Convert body position to topocentric
	body[0]-=observerPV[0][0];
	body[1]-=observerPV[0][1];
	body[2]-=observerPV[0][2];

	//Convert coords to polar, which gives RA/DEC
	double r = sqrt(body[0] * body[0] + body[1] * body[1] + body[2] * body[2]);
	double dec = acos(body[2] / r);
	double ra = atan2(body[1], body[0]);

	if(ra<0) ra+=2*PI;
	if(dec<0) dec+=2*PI;
	dec=.5*PI-dec;

	printf("Moon: %f %f\r\n",ra*180.0/PI,dec*180.0/PI);
	printf("Diff: %f %f\r\n",ra*180.0/PI-expectedRA,dec*180.0/PI-(expectedDec));

	//Convert to altaz
	//double GMST=iauGst06a(utc1,utc2,tt1,tt2);
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
	printf("Diff  : %f %f\r\n",alt*180.0/PI-expectedAlt,az*180.0/PI-expectedAz);


}
