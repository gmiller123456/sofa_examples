#include "astrolib.h"

void getBody(int bodyNum,double et, double body[3]){
	double earth[3];
	double emb[3];

	switch(bodyNum){
		case 0:
			body[0]=0;
			body[1]=0;
			body[2]=0;
			return; //Sun is at the center for vsop87a
			//return vsop87e_full.getSun(et);  // "E" is the only version the Sun is not always at [0,0,0]
		case 1:
			return vsop87a_full_getMercury(et,body);
		case 2:
			return vsop87a_full_getVenus(et,body);
		case 3:
			return vsop87a_full_getEarth(et,body);
		case 4:
			return vsop87a_full_getMars(et,body);
		case 5:
			return vsop87a_full_getJupiter(et,body);
		case 6:
			return vsop87a_full_getSaturn(et,body);
		case 7:
			return vsop87a_full_getUranus(et,body);
		case 8:
			return vsop87a_full_getNeptune(et,body);
		case 9:
			//return [0,0,0]; //Vsop87a is the only version which can compute the moon
			return vsop87a_full_getEmb(et,body);
		case 10:
			//return [0,0,0]; //Vsop87a is the only version which can compute the moon
			vsop87a_full_getEmb(et,emb);
			vsop87a_full_getEarth(et,earth);
			return vsop87a_full_getMoon(earth, emb, body);
	}
}

void rotvsop2J2000(double body[3]){
	double a,b,c;
	a = body[0] + body[1] * 0.000000440360 + body[2] * -0.000000190919;
	b = body[0] * -0.000000479966 + body[1] * 0.917482137087 + body[2] * -0.397776982902;
	c = body[1] * 0.397776982902 + body[2] * 0.917482137087;

	body[0]=a;
	body[1]=b;
	body[2]=c;
}
