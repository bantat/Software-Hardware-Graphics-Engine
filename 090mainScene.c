/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing
script.
Run the script like so  clang 090mainScene.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include "070vector.c"
#include "090matrix.c"
#include "040texture.c"

#define renVARYDIMBOUND 16
#define renVERTNUMBOUND 300

#include "090renderer.c"

#define renVARYX 0
#define renVARYY 1
#define renVARYS 2
#define renVARYT 3
#define renVARYR 4
#define renVARYG 5
#define renVARYB 6
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2
#define renUNIFTHETA 0
#define renUNIFTRANSX 1
#define renUNIFTRANSY 2
#define renUNIFISOMETRY 3


#define renATTRX 0
#define renATTRY 1
#define renATTRS 2
#define renATTRT 3
#define renATTRR 4
#define renATTRG 5
#define renATTRB 6
#define degree (M_PI/180)

//double unif[3] = { 0, 100, 100 };
double unif[12];

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */

    double R[2][2];
    double attrXYvec [2];
    double RtimesXYvec[2];
    double T[2];

    double colP0[2] = { cos(unif[0]) , sin(unif[0])};
    double colP1[2] = { (-1)*sin(unif[0]) , cos(unif[0])};

    mat22Columns(colP0,colP1,R);
    //mat22Print(R);

    attrXYvec[0] = attr[renATTRX];
    attrXYvec[1] = attr[renATTRY];

    mat22Multiply(R,attrXYvec,RtimesXYvec);

    T[0] = unif[1];
    T[1] = unif[2];
    vecAdd(2,T,RtimesXYvec,vary);

    //vary[renVARYX] = attr[renATTRX];
    //vary[renVARYY] = attr[renATTRY];
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];

}

/* If unifParent is NULL, then sets the uniform matrix to the
rotation-translation M described by the other uniforms. If unifParent is not
NULL, but instead contains a rotation-translation P, then sets the uniform
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    if (unifParent == NULL)
        /* The nine uniforms for storing the matrix start at index
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX],
            unif[renUNIFTRANSY], (double(*)[3])(&unif[renUNIFISOMETRY]));
    else {
        double m[3][3];
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX],
            unif[renUNIFTRANSY], m);
        mat333Multiply((double(*)[3])(&unifParent[renUNIFISOMETRY]), m,
            (double(*)[3])(&unif[renUNIFISOMETRY]));
    }
}

/* Sets rgb, based on the other parameters, which are unaltered. attr is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
                double vary[], double rgb[]) {
  texSample(tex[0], vary[renVARYS], vary[renVARYT]);
  rgb[0] = tex[0]->sample[renTEXR];
  rgb[1] = tex[0]->sample[renTEXG];
  rgb[2] = tex[0]->sample[renTEXB];
}

#include "090triangle.c"
#include "090mesh.c"

int filter = 0;
texTexture *tex[1];
renRenderer ren;
meshMesh mesh;


void handleKeyUp(int button, int shiftIsDown, int controlIsDown,
                 int altOptionIsDown, int superCommandIsDown) {
  if (button == 257) {
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
  pixClearRGB(0,0,0);
  meshRender(&mesh, &ren, unif, tex);
  //triRender(&ren, unif, tex, a, b, c);
}

void handleTimeStep(double oldTime, double newTime) {
  if (floor(newTime) - floor(oldTime) >= 1.0)
    printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
  unif[0] += 3*(newTime - oldTime);
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
    texInitializeFile(&texture, "aliens.jpg");
    tex[0] = &texture;
    ren.attrDim = 4;
    ren.varyDim = 4;
    ren.texNum = 1;
    ren.unifDim = 3;

    ren.colorPixel = colorPixel;
    ren.transformVertex = transformVertex;

    pixSetTimeStepHandler(handleTimeStep);
    pixSetKeyUpHandler(handleKeyUp);
    meshInitializeEllipse(&mesh, 0.0, 0.0, 100.0, 200.0, 40);
    draw();
    pixRun();

    texDestroy(tex[0]);
    meshDestroy(&mesh);

    return 0;
  }
}
