/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the function that renders a triangle by rasterisation. It also handles triangle coodinates
given in any order.
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"


/*
@function MAX
@param (double,double,double)
@purpose Finds the Maximum value of the three doubles given as parameters and returns it.
*/
double MAX( a, b, c ){
	return ( ( a > b) ? ((a>c)? a : c): ((b > c) ? b : c ));
}

/*
@function triRender
@param (double a0,double a1,double b0,double b1,double c0,double c1,double r,double g,double b), where
a0,...,b1 are coodinates of the triangle and r,g,b represent the color of the triangle to be drawn.
@purpose This function first normalises whatever triangle coodinates it is given. It then renders
the triangle according to the equations of lines. It also has some cotrol logic for different triangle shapes
and situations.
*/

void triRender(double a[2], double b[2], double c[2],double rgb[3]){
	// initialising the normalised triangle coodinates
	double xleft,yleft,xmid,ymid,xright,yright;

	//Find the maximum x to figure out which is the right most coodinate
	double  m = MAX(a[0],b[0],c[0]);

	// debugging print statement: printf("this is m ======%f\n",m );

	//Do the normalisation of the triangle by reassigning triangle coodinate values and attributes
	if (m == c[0]){

		//This case c0 is the right most point so reassign
		xright = c[0];
		yright = c[1];

			//find the mid point and reassign
			if (b[0] == MAX(a[0],b[0],0)){
				xmid = b[0];
				ymid = b[1];
				xleft = a[0];
				yleft = a[1];
			}else{
				xmid = a[0];
				ymid = a[1];
				xleft = b[0];
				yleft = b[1];
			}

	}else if(m==b[0]){
		//b0 is the right most point
		xright = b[0];
		yright = b[1];
			//finding the mid and the left most
			if (c[0] ==MAX(a[0],c[0],0)){
				xmid = c[0];
				ymid = c[1];
				xleft = a[0];
				yleft = a[1];
			}else{
				xmid = a[0];
				ymid = a[1];
				xleft = c[0];
				yleft = c[1];
			}
	}else{
		//a0 is the right most x coodinate
		xright = a[0];
		yright = a[1];

			if (b[0] == MAX(b[0],c[0],0)){
				xmid = b[0];
				ymid = b[1];
				xleft = c[0];
				yleft = c[1];
			}else{
				xmid = c[0];
				ymid = c[1];
				xleft = b[0];
				yleft = b[1];
			}
	}

	//figure out if the mid point is above line (left most) ---> (right most), teslls us the shape of triangle
	int shp = (ymid < MAX(yleft,yright,0) ? 1 : 2);

	//this variable changes if the triangle is right angled so we dont devide by zero
	int rght = 0;
	//Figuring out if the triangle is right angled
	if (xleft == xmid){
		//straight line on the left side
		rght = 1;
	}else if(xmid == xright){
		//straight line on the right side
		rght = 2;
	}



	//check which one is the middle one
	//debug and check if the the coodinates are correctly normalised, shape is correct and equal x-coodinates detected
	printf("xleft = %f, yleft= %f, xmid = %f,ymid= %f, xright = %f,yright = %f,shape = %d,right angled = %d\n"
		,xleft,yleft,xmid,ymid,xright,yright,shp,rght);



	//Now draw the first half of the triangle
	//For every x coodinate between left most point to mid point
	for(int i = ceil(xleft); i <= floor(xmid);i++){
		double ylow,yhigh;

		//If the mid point is above and (xleft is not inline with xmid or xleft)
		if (shp == 2 && !(rght == 1)){
			ylow = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			yhigh = yleft+(((ymid-yleft)/(xmid-xleft))*(i-xleft));

		//If the mid point is below and xleft is not inline with xmid
		}else if (shp == 1 && !(rght == 1)){
			yhigh = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			ylow = yleft+(((ymid-yleft)/(xmid-xleft))*(i-xleft));
		}

		//draw the triangle
		for (int j=ceil(ylow);j <= floor(yhigh);j++){
			pixSetRGB(i,j, rgb[0], rgb[1], rgb[2]);
		}

	}

	//from xmid to xright
	for(int i = floor(xmid); i <= (floor(xright));i++){
		double ylow,yhigh;

		//if mid point is above and xmid is not inline with xright
		if (shp == 2 && !(rght == 2)){
			ylow = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			yhigh = ymid+(((yright-ymid)/(xright-xmid))*(i-xmid));
		//if mid point is below and xmid is not inline with xright
		}else if (shp == 1 && !(rght == 2)){
			yhigh = yleft+(((yright-yleft)/(xright-xleft))*(i-xleft));
			ylow = ymid+(((yright-ymid)/(xright-xmid))*(i-xmid));
		}


		for (int j=ceil(ylow);j <= floor(yhigh);j++){
			//Debugg statement : printf("%d,%d,%f,%f\n",i,j,ylow,yhigh );
			pixSetRGB(i,j, rgb[0], rgb[1], rgb[2]);
		}

	}


}
