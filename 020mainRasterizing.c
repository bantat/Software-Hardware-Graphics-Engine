
#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include "020triangle.c"

int main(void){

	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else{

		triRender( 	100,400,
					200,100,
					400,450,
					0.0,0.0,1.0);
		pixRun();
		return 0;
	}

}