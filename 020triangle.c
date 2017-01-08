
#include <stdio.h>
#include <math.h>
#include "000pixel.h"

double MAX( a, b, c ){

	return ( ( a > b) ? ((a>c)? a : c): ((b > c) ? b : c ));

}

void triRender(double a0,double a1,double b0,double b1,double c0,double c1,double r,double g,double b){
	//might need to normalise the triangle always on here make sure cO is the furtherst point.

	double xleft,yleft,xmid,ymid,xright,yright;

	int shp = 0;

	double  m = MAX(a0,b0,c0);
	printf("this is m ======%f\n",m );
	//assign the outermost corner
	if (m == c0){
		printf("%s\n","was in here" );
		xright = c0;
		yright = c1;

				if (b0 == MAX(a0,b0,0)){
					xmid = b0;
					ymid = b1;
					xleft = a0;
					yleft = a1;
				}else{
					xmid = a0;
					ymid = a1;
					xleft = b0;
					yleft = b1;
				}

	}else if(m==b0){
		printf("%s\n","was in here" );
			xright = b0;
			yright = b1;

				if (c0 ==MAX(a0,c0,0)){
					xmid = c0;
					ymid = c1;
					xleft = a0;
					yleft = a1;
				}else{
					xmid = a0;
					ymid = a1;
					xleft = c0;
					yleft = c1;
				}		
	}else{

			xright = a0;
			yright = a1;

				if (b0 == MAX(b0,c0,0)){
					xmid = b0;
					ymid = b1;
					xleft = c0;
					yleft = c1;
				}else{
					xmid = c0;
					ymid = c1;
					xleft = b0;
					yleft = b1;
				}				
			}

	shp = (ymid < MAX(yleft,yright,0) ? 1 : 2);

	//check which one is the middle one 
	printf("xleft = %f, yleft= %f, xmid = %f,ymid= %f, xright = %f,yright = %f,shape = %d\n",xleft,yleft,xmid,ymid,xright,yright,shp);




	for(int i = ceil(xleft); i <= floor(xmid);i++){
		double ylow,yhigh;
		if (shp == 2 ){
			ylow = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			yhigh = yleft+(((ymid-yleft)/(xmid-xleft))*(i-xleft));		
		}else{
			yhigh = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			ylow = yleft+(((ymid-yleft)/(xmid-xleft))*(i-xleft));
		}

		for (int j=ceil(ylow);j <= floor(yhigh);j++){
			pixSetRGB(i,j, r, g, b);
		}

	}

	for(int i = floor(xmid); i <= (floor(xright));i++){

		double ylow,yhigh;
		if (shp == 2){
			ylow = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			yhigh = ymid+(((yright-ymid)/(xright-xmid))*(i-xmid));			
		}else{
			yhigh = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			ylow = ymid+(((yright-ymid)/(xright-xmid))*(i-xmid));
		}


		for (int j=ceil(ylow);j <= floor(yhigh);j++){
			printf("%d,%d,%f,%f\n",i,j,ylow,yhigh );
			pixSetRGB(i,j, r, g, b);
		}

	}

	
}





