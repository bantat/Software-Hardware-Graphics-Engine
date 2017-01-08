/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing script. 
Run the script like so  clang 020mainRasterizing.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include "020triangle.c"


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
		triRender( 	100,400,
					100,100,
					250,450,
					0.0,0.0,1.0);
		pixRun();
		return 0;
	}

}