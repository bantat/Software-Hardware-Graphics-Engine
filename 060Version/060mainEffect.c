/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing
script.
Run the script like so  clang 060mainEffect.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include "030vector.c"
#include "030matrix.c"
#include "040texture.c"

#define renATTRDIMBOUND 16

#include "050renderer.c"

#define renATTRX 0
#define renATTRY 1
#define renATTRS 2
#define renATTRT 3
#define renATTRR 4
#define renATTRG 5
#define renATTRB 6
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

double y_val = 0;

/* Sets rgb, based on the other parameters, which are unaltered. attr is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
                double attr[], double rgb[]) {
  texSample(tex[0], attr[renATTRS], attr[renATTRT]);
  texSample(tex[1], attr[renATTRS], attr[renATTRT] + y_val);
  rgb[0] = tex[0]->sample[renTEXR] * unif[renUNIFR] * attr[renATTRR] * tex[1]->sample[renTEXR];
  rgb[1] = tex[0]->sample[renTEXG] * unif[renUNIFG] * attr[renATTRG] * tex[1]->sample[renTEXR];
  rgb[2] = tex[0]->sample[renTEXB] * unif[renUNIFB] * attr[renATTRB] * tex[1]->sample[renTEXR];
}

#include "050triangle.c"

int filter = 0;
texTexture *tex[2];
renRenderer ren;


void handleKeyUp(int button, int shiftIsDown, int controlIsDown,
                 int altOptionIsDown, int superCommandIsDown) {
  if (button == 257) {
    // printf("filter: %d\n", filter);
    if (filter == 0) {
      texSetFiltering(tex[0], texQUADRATIC);
      filter = 1;
    } else {
      texSetFiltering(tex[0], texNEAREST);
      filter = 0;
    }
  }
}

void draw() {
  double a[renATTRDIMBOUND] = {0,512,0.0,1.0,1.0,1.0,1.0,1.0};
  double b[renATTRDIMBOUND] = {512,0,1.0,0.0,1.0,1.0,1.0,1.0};
  double c[renATTRDIMBOUND] = {512,512,1.0,1.0,1.0,1.0,1.0,0.8};
  double unif[4] = {0.8, 1.0, 0.9, 1.0};
  /*
  double a[2] = {300, 150};
  double b[2] = {50, 100};
  double c[2] = {200, 450};
  double
  double alpha[2] = {0.0, 0.0};
  double beta[2] = {0.5, 0.0};
  double gamma[2] = {0.8, 0.75};
  */
  triRender(&ren, unif, tex, a, b, c);
}

void handleTimeStep(double oldTime, double newTime) {
  if (floor(newTime) - floor(oldTime) >= 1.0)
    printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
  y_val = y_val + 0.01;
  draw();
}

/*
@function main
@param void
@purpose This function initialises the GL window with the specified width and
hieght and then calls the
triRender function in 020triangle.c . This is being used to test if triRender
works fine.
*/
int main(void) {

  if (pixInitialize(512, 512, "Pixel Graphics") != 0)
    return 1;
  else {
    texTexture texture;
    texTexture texture1;
    texInitializeFile(&texture, "aliens.jpg");
    texInitializeFile(&texture1, "static.jpg");
    texSetTopBottom(&texture1, texREPEAT);
    tex[0] = &texture;
    tex[1] = &texture1;
    ren.attrDim = 8;
    ren.texNum = 2;
    ren.unifDim = 4;

    draw();

    pixSetTimeStepHandler(handleTimeStep);
    pixSetKeyUpHandler(handleKeyUp);
    // texInitializeFile(&tex, "wall.jpg");
    texDestroy(tex[0]);
    texDestroy(tex[1]);
    pixRun();
    return 0;
  }
}
