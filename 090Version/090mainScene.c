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

double x_val = 0.0;
#define renATTRX 0
#define renATTRY 1
#define renATTRS 2
#define renATTRT 3
#define renATTRR 4
#define renATTRG 5
#define renATTRB 6

//double unif[3] = { 0, 100, 100 };
double unif[12] = {0.0,0.0,0.0};

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */
    double attrXYvec [3] = {attr[renATTRX],attr[renATTRY],1};
    double RtimesXYvec[3];
    mat331Multiply((double(*)[3])(&unif[renUNIFISOMETRY]),attrXYvec,RtimesXYvec);
    vary[renVARYX] = RtimesXYvec[0];
    vary[renVARYY] = RtimesXYvec[1];
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

/* If unifParent is NULL, then sets the uniform matrix to the
rotation-translation M described by the other uniforms. If unifParent is not
NULL, but instead contains a rotation-translation P, then sets the uniform
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    if (unifParent == NULL){
        /* The nine uniforms for storing the matrix start at index
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX],
            unif[renUNIFTRANSY], (double(*)[3])(&unif[renUNIFISOMETRY]));
    } else {
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
  texSample(tex[0], vary[renVARYS]+x_val, vary[renVARYT]);
  //printf("done\n");
  rgb[0] = tex[0]->sample[renTEXR];
  rgb[1] = tex[0]->sample[renTEXG];
  rgb[2] = tex[0]->sample[renTEXB];
    //printf("here we are\n");
}

#include "090triangle.c"
#include "090mesh.c"
#include "090scene.c"

int filter = 0;
texTexture * tex[3];
renRenderer ren;
sceneNode scen0;
sceneNode scen1;
sceneNode scen2;
meshMesh mesh0;
meshMesh mesh1;
meshMesh mesh2;



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
  sceneRender(&scen0,&ren,NULL);
}

void handleTimeStep(double oldTime, double newTime) {
  if (floor(newTime) - floor(oldTime) >= 1.0)
    printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    x_val += 0.01;
    unif[0] = unif[0] + 0.5;
    sceneSetUniform(&scen2,&ren,unif);
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
    texTexture texture0, texture1, texture2;
    texInitializeFile(&texture0, "ocean.png");
    texInitializeFile(&texture1, "wall.jpg");
    texInitializeFile(&texture2, "sun.png");
    tex[0] = &texture0, tex[1] = &texture1, tex[2] = &texture2;

    ren.attrDim = 4;
    ren.varyDim = 4;
    ren.texNum = 1;
    ren.unifDim = 12;
    ren.colorPixel = colorPixel;
    ren.transformVertex = transformVertex;
    ren.updateUniform = updateUniform;
    texSetLeftRight(&texture0, texREPEAT);

    pixSetTimeStepHandler(handleTimeStep);
    pixSetKeyUpHandler(handleKeyUp);



    meshInitializeRectangle(&mesh0,0.0,512.0,0.0,512.0);
    meshInitializeEllipse(&mesh1, 300.0, 300.0, 30.0, 30.0, 50);
    meshInitializeEllipse(&mesh2, 100.0, 400.0, 50.0, 50.0, 50);

    sceneInitialize(&scen0,&ren,unif,tex,&mesh0,NULL,NULL);
    sceneInitialize(&scen1,&ren,unif,tex,&mesh1,NULL,NULL);
    sceneInitialize(&scen2,&ren,unif,tex,&mesh2,NULL,NULL);

    // sceneSetTexture(&scen0,&ren,0,tex[0]);
    sceneSetTexture(&scen1,&ren,0,tex[1]);
    sceneSetTexture(&scen2,&ren,0,tex[2]);
    sceneAddChild(&scen0,&scen1);
    sceneAddChild(&scen1,&scen2);



    draw();
    pixRun();

    texDestroy(tex[0]);
    meshDestroy(&mesh0);
    texDestroy(tex[1]);
    meshDestroy(&mesh1);
    texDestroy(tex[2]);
    meshDestroy(&mesh2);
    sceneDestroyRecursively(&scen0);



    return 0;
  }
}
