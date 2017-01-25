/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing
script.
Run the script like so  clang 100main3D.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"

#include "100vector.c"
#include "100matrix.c"
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
//#define renUNIFR 0
//#define renUNIFG 1
//#define renUNIFB 2
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2
#define renUNIFRHO 0
#define renUNIFPHI 1
#define renUNIFTHETA 2
#define renUNIFTRANSX 3
#define renUNIFTRANSY 4
#define renUNIFTRANSZ 5
#define renUNIFISOMETRY 6

double x_val = 0.0;
#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRR 5
#define renATTRG 6
#define renATTRB 7

double unif[22] = {0.5,0.6,0.7,0.0,0.0,0.0, 1.0,0.0,0.0,0.0,
                                             0.0,1.0,0.0,0.0,
                                             0.0,0.0,1.0,0.0,
                                             0.0,0.0,0.0,1.0};

double unif2[22] = {0.0,0.0,0.0,250.0,250.0,0.0, 1.0,0.0,0.0,0.0,
                                            0.0,1.0,0.0,0.0,
                                            0.0,0.0,1.0,0.0,
                                            0.0,0.0,0.0,1.0};
/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */
    double attrXYZvec[4] = {attr[renATTRX],attr[renATTRY],attr[renATTRZ],1};
    double RtimesXYZvec[4];
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]),attrXYZvec,RtimesXYZvec);
    vary[renVARYX] = RtimesXYZvec[0];
    vary[renVARYY] = RtimesXYZvec[1];
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

/* If unifParent is NULL, then sets the uniform matrix to the
rotation-translation M described by the other uniforms. If unifParent is not
NULL, but instead contains a rotation-translation P, then sets the uniform
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
  double u[3];
  double rot[3][3];

  vec3Spherical(unif[renUNIFRHO],unif[renUNIFPHI],unif[renUNIFTHETA],u);
  mat33AngleAxisRotation(unif[renUNIFRHO],u,rot);

  if (unifParent == NULL) {

      /* The nine uniforms for storing the matrix start at index
      renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those
      nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */



      double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], unif[renUNIFTRANSZ]};

      mat44Isometry(rot, trans, (double(*)[4])(&unif[renUNIFISOMETRY]));

      //double trans[3] = {unif[renUNIFTRANSX],unif[renUNIFTRANSY],unif[renUNIFTRANSZ]};
      //mat44Isometry((double(*)[4])(&unif[renUNIFISOMETRY]), trans,
        //(double(*)[4])(&unif[renUNIFISOMETRY]));
  } else {

    double m[4][4];

    double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], unif[renUNIFTRANSZ]};

    mat44Isometry(rot, trans, m);

    mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m,
        (double(*)[4])(&unif[renUNIFISOMETRY]));

    /*
      double m[4][4];
      mat44Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX],
          unif[renUNIFTRANSY], m);
      mat333Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m,
          (double(*)[4])(&unif[renUNIFISOMETRY]));
          */
  }
}

/* Sets rgb, based on the other parameters, which are unaltered. attr is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
                double vary[], double rgb[]) {
  texSample(tex[0], vary[renVARYS], vary[renVARYT]);
  //printf("done\n");
  rgb[0] = tex[0]->sample[renTEXR];
  rgb[1] = tex[0]->sample[renTEXG];
  rgb[2] = tex[0]->sample[renTEXB];
    //printf("here we are\n");
}

//#include "090triangle.c"
#include "091triangle.c"
#include "100mesh.c"
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

  pixClearRGB(0.0,0.0,0.0);
  sceneRender(&scen0,&ren,NULL);
}

void handleRotation() {
  //scen0.unif[renUNIFTHETA] = scen0.unif[renUNIFTHETA] + 0.05;
  scen0.unif[renUNIFTHETA] = scen0.unif[renUNIFTHETA] + 0.05;

}

void handleTimeStep(double oldTime, double newTime) {
  if (floor(newTime) - floor(oldTime) >= 1.0)
    printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

    // change rho

    handleRotation();
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

    ren.attrDim = 8;
    ren.varyDim = 4;
    ren.texNum = 1;
    ren.unifDim = 22;
    ren.colorPixel = colorPixel;
    ren.transformVertex = transformVertex;
    ren.updateUniform = updateUniform;
    //texSetLeftRight(&texture0, texREPEAT);

    pixSetTimeStepHandler(handleTimeStep);
    pixSetKeyUpHandler(handleKeyUp);

    texSetLeftRight(&texture1, texREPEAT);
    texSetTopBottom(&texture1, texREPEAT);



    //meshInitializeRectangle(&mesh0,0.0,512.0,0.0,512.0);
    //meshInitializeEllipse(&mesh1, 300.0, 300.0, 30.0, 30.0, 50);
    //meshInitializeEllipse(&mesh2, 0.0, 0.0, 50.0, 50.0, 50);

    meshInitializeBox(&mesh0, 150.0, 250.0, 150.0, 250.0, 150.0, 250.0);
    meshInitializeSphere(&mesh1, 30, 20, 20);
    sceneInitialize(&scen0,&ren,unif,tex,&mesh0,NULL,NULL);
    sceneInitialize(&scen1,&ren,unif,tex,&mesh1,NULL,NULL);
    //sceneInitialize(&scen2,&ren,unif,tex,&mesh2,NULL,NULL);

    sceneSetTexture(&scen1,&ren,0,tex[1]);
    //sceneSetTexture(&scen1,&ren,0,tex[1]);
    //sceneSetTexture(&scen2,&ren,0,tex[2]);
    sceneSetUniform(&scen1,&ren,unif2);
    sceneAddChild(&scen0,&scen1);
    //sceneAddChild(&scen1,&scen2);



    draw();
    pixRun();


    texDestroy(tex[0]);
    meshDestroy(&mesh0);
    //texDestroy(tex[1]);
    //meshDestroy(&mesh1);
    //texDestroy(tex[2]);
    //meshDestroy(&mesh2);
    sceneDestroyRecursively(&scen0);



    return 0;
  }
}
