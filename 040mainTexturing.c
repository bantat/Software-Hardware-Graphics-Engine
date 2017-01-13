/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing script.
Run the script like so  clang 040mainTexturing.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include "040triangle.c"
#include "030vector.c"
#include "030matrix.c"
//#include "040texture.c"


int filter = 0;
texTexture tex;

void handleKeyDown(int button, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (button == 257) {
		//printf("filter: %d\n", filter);
		if (filter == 0) {
			texSetFiltering(&tex, texQUADRATIC);
			filter = 1;
		} else {
			texSetFiltering(&tex, texNEAREST);
			filter = 0;
		}
	}
}

void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

		double a[2] = {400,50};
		double b[2] = {50,100};
		double c[2] = {200,450};
		double rgb[3] = {1.0,1.0,1.0};
		double alpha[2] = {0.0,0.0};
		double beta[2] = {0.5,0.0};
		double gamma[2] = {0.8,0.75};
		triRender(a,b,c,rgb,&tex,alpha,beta,gamma);
}

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
		pixSetTimeStepHandler(handleTimeStep);
		pixSetKeyDownHandler(handleKeyDown);
		texInitializeFile(&tex, "wall.jpg");
		texDestroy(&tex);
		pixRun();
		return 0;
	}

}
