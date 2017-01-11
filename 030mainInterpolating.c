/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing script.
Run the script like so  clang 030mainInterpolating.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include "030triangle.c"
#include "030vector.c"
#include "030matrix.c"


/*
@function main
@param void
@purpose This function initialises the GL window with the specified width and hieght and then calls the
triRender function in 020triangle.c . This is being used to test if triRender works fine.
*/
int main(void){

	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else{
		//calling trirender with the coodinates of the triangle
		double a[2] = {400,50};
		double b[2] = {50,100};
		double c[2] = {200,450};
		double rgb[3] = {1.0,1.0,1.0};
		double alpha[3] = {0.0,0.0,1.0};
		double beta[3] = {0.0,1.0,0.0};
		double gamma[3] = {1.0,0.0,0.0};

		triRender(a,b,c,rgb,alpha,beta,gamma);
		pixRun();
		return 0;
	}

}
