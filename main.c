#include <math.h>
#include <stdio.h>
#include "sofa/sofa.h"
#include "vsop87a_full.h"
#include "reductiontest1.h"


#define PI M_PI

void simpleReduction();

int main(int argc, char* args[]){
	simpleReduction();
	return 0;
}



void tests(){
	printf("Here\r\n");
	double era=iauEra00(2451545.0,0);
	printf("%f\r\n",era);

	double jd1;
	double jd2;
	iauCal2jd(2000,1,1,&jd1,&jd2);
	printf("%f %f\r\n",jd1,jd2);

	//convert Date to utc
	double utc1,utc2;
	iauDtf2d("UTC",2000,1,1,0,0,0,&utc1,&utc2);
	printf("%f %f\r\n",utc1,utc2);

	//convert UTC to TAI
	double atomic1,atomic2;
	iauUtctai(utc1,utc2,&atomic1,&atomic2);
	printf("%f %f\r\n",atomic1,atomic2);

	//convert TAI to TT
	double tt1,tt2;
	iauTaitt(atomic1,atomic2,&tt1,&tt2);
	printf("%f %f\r\n",tt1,tt2);

	//Convert back to components for display
	int ttyear,ttmonth,ttday,ihmsf[4];
	iauD2dtf("tt",3,tt1,tt2,&ttyear,&ttmonth,&ttday,ihmsf);
	printf ( "%4d/%2.2d/%2.2d%3d:%2.2d:%2.2d.%3.3d\n", ttyear, ttmonth, ttday,ihmsf[0], ihmsf[1], ihmsf[2], ihmsf[3] );

	double precessionMatrix[3][3];
	iauPmat06(tt1,tt2,precessionMatrix);
	printf("Precession Matrix:\r\n%f %f %f\r\n%f %f %f\r\n%f %f %f\r\n",
			precessionMatrix[0][0],precessionMatrix[0][1],precessionMatrix[0][2],
			precessionMatrix[1][0],precessionMatrix[1][1],precessionMatrix[1][2],
			precessionMatrix[2][0],precessionMatrix[2][1],precessionMatrix[2][2]
			);

	double observerPV[2][3];
	iauPvtob(274.236400*PI/180.0,38.2464000*PI/180.0,0,0,0,0,era,observerPV);
	printf("Observer Position and Velocity:\r\n%f %f %f\r\n%f %f %f\r\n",
			observerPV[0][0],observerPV[0][1],observerPV[0][2],
			observerPV[1][0],observerPV[1][1],observerPV[1][2]
			);



}
