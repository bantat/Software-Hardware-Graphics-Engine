/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the main function that test the 020triangle.c rasterizing
script.
Run the script like so  clang 140mainClipping.c 000pixel.o -lglfw -framework OpenGL
*/

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"

#include "100vector.c"
#include "130matrix.c"
#include "040texture.c"
#include "110depth.c"

#define GLFW_KEY_ENTER 257
#define GLFW_KEY_RIGHT 262
#define GLFW_KEY_LEFT 263
#define GLFW_KEY_DOWN 264
#define GLFW_KEY_UP 265
#define GLFW_KEY_KP_ADD 334
#define GLFW_KEY_KP_SUBTRACT 333
#define GLFW_KEY_W 87
#define GLFW_KEY_S 83

#define renVARYDIMBOUND 16
#define renVERTNUMBOUND 300

#include "130renderer.c"

#define renVARYX 0
#define renVARYY 1
#define renVARYZ 2
#define renVARYW 3
#define renVARYS 4
#define renVARYT 5
#define renVARYR 6
#define renVARYG 7
#define renVARYB 8
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
#define renUNIFVIEWING 22

double x_val = 0.0;
#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRR 5
#define renATTRG 6
#define renATTRB 7

//double cam[2] = {M_PI/2,-1*M_PI/2};
//double cam[2] = {M_PI/2,0.0};
double cam[3] = {0.0,0.0,100.0};

double target[3] = {0.0, 0.0, 0.0};
///////////////////////1.0,1.6
double unif[38] = {0.0,0.0,0.0,0.0,0.0,0.0,     1.0,0.0,0.0,0.0,
                                                0.0,1.0,0.0,0.0,
                                                0.0,0.0,1.0,0.0,
                                                0.0,0.0,0.0,1.0, 	0.0,0.0,0.0,0.0,
                                                                 	0.0,0.0,0.0,0.0,
                                                                 	0.0,0.0,0.0,0.0,
                                                                 	0.0,0.0,0.0,0.0};
double unif2[38] = {0.0,0.0,0.0,0.0,0.0,-30.0, 1.0,0.0,0.0,0.0,
                                              0.0,1.0,0.0,0.0,
                                              0.0,0.0,1.0,0.0,
                                              0.0,0.0,0.0,1.0, 0.0,0.0,0.0,0.0,
                                                               0.0,0.0,0.0,0.0,
                                                               0.0,0.0,0.0,0.0,
                                                               0.0,0.0,0.0,0.0};

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    /* For now, just copy attr to varying. Baby steps. */
    double attrXYZvec[4] = {attr[renATTRX],attr[renATTRY],attr[renATTRZ],1};
    double RtimesXYZvec[4];
    double MtimesRvec[4];

    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]),attrXYZvec,RtimesXYZvec);
    mat441Multiply((double(*)[4])(&unif[renUNIFVIEWING]),RtimesXYZvec,MtimesRvec);

    vary[renVARYX] = MtimesRvec[0];
    vary[renVARYY] = MtimesRvec[1];
    vary[renVARYZ] = MtimesRvec[2];
    vary[renVARYW] = MtimesRvec[3];
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

  mat44Copy(ren->viewing, (double(*)[4])(&unif[renUNIFVIEWING]));

  vec3Spherical(1.0,unif[renUNIFPHI],unif[renUNIFTHETA],u);
  mat33AngleAxisRotation(unif[renUNIFRHO],u,rot);

  if (unifParent == NULL) {

      /* The nine uniforms for storing the matrix start at index
      renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those
      nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
      double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], unif[renUNIFTRANSZ]};
      mat44Isometry(rot, trans, (double(*)[4])(&unif[renUNIFISOMETRY]));

  } else {

    double m[4][4];
    double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], unif[renUNIFTRANSZ]};
    mat44Isometry(rot, trans, m);
    mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m,
        (double(*)[4])(&unif[renUNIFISOMETRY]));
  }
}

/* Sets rgb, based on the other parameters, which are unaltered. attr is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
                double vary[], double rgbz[]) {
  texSample(tex[0], vary[renVARYS], vary[renVARYT]);
  //printf("done\n");
  rgbz[0] = tex[0]->sample[renTEXR];
  rgbz[1] = tex[0]->sample[renTEXG];
  rgbz[2] = tex[0]->sample[renTEXB];
  rgbz[3] = depthGetZ(ren->depth, vary[renVARYX], vary[renVARYY]);
    //printf("here we are\n");
}

#include "110triangle.c"
#include "140clipping.c"
#include "140mesh.c"
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
depthBuffer dep;



void handleKeyUp(int button, int shiftIsDown, int controlIsDown,
                 int altOptionIsDown, int superCommandIsDown) {
  if (button == GLFW_KEY_ENTER) {
    if (filter == 0) {
      texSetFiltering(tex[0], texQUADRATIC);
      filter = 1;
    } else {
      texSetFiltering(tex[0], texNEAREST);
      filter = 0;
    }

  } else if (button == GLFW_KEY_UP) {
    if (cam[0] - 0.05 < 0.0) {
      cam[0] = M_PI;
    } else {
      cam[0] = cam[0] - 0.05;
    }
  } else if (button == GLFW_KEY_DOWN) {
    if (cam[0] + 0.05 > M_PI) {
      cam[0] = 0.0;
    } else {
      cam[0] = cam[0] + 0.05;
    }

  } else if (button == GLFW_KEY_LEFT) {
    if (cam[1] - 0.05 < (-1*M_PI)) {
      cam[1] = M_PI - 0.05;
    } else {
      cam[1] = cam[1] - 0.05;
    }
  } else if (button == GLFW_KEY_RIGHT) {
    if (cam[1] + 0.05 > M_PI) {
      cam[1] = (-1*M_PI) + 0.05;
    } else {
      cam[1] = cam[1] + 0.05;
    }
  } else if (button == GLFW_KEY_KP_ADD || button == GLFW_KEY_W ) {
    cam[2] = cam[2] + 5.0;
  } else if (button == GLFW_KEY_KP_SUBTRACT || button == GLFW_KEY_S ) {
    if (cam[2] - 5.0 < 5.0) {
      return;
    } else {
      cam[2] = cam[2] - 5.0;
    }
  }
}


void draw() {
  renUpdateViewing(&ren);
  depthClearZs(&dep,-1000);
  pixClearRGB(0.0,0.0,0.0);
  sceneRender(&scen0,&ren,NULL);
}

void handleRotation() {

  renLookAt(&ren, target, cam[2], cam[0], cam[1]);

}

void handleTimeStep(double oldTime, double newTime) {
  if (floor(newTime) - floor(oldTime) >= 1.0)
    printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    //printf("[%f, %f]\n", cam[0], cam[1]);
    handleRotation();
    //printf("cam: %f\n", cam[2]);
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

    texTexture texture0, texture1, texture2, texture3;
    texInitializeFile(&texture0, "box.jpg");
    texInitializeFile(&texture1, "beachball.jpg");

    depthInitialize(&dep,512,512);
    tex[0] = &texture0;
    tex[1] = &texture1;

    ren.attrDim = 8;
    ren.varyDim = 6;
    ren.texNum = 1;
    ren.unifDim = 38;
    ren.colorPixel = colorPixel;
    ren.transformVertex = transformVertex;
    ren.updateUniform = updateUniform;
    ren.depth = &dep;

    texSetLeftRight(&texture0, texREPEAT);
    texSetTopBottom(&texture0, texREPEAT);

    pixSetTimeStepHandler(handleTimeStep);
    pixSetKeyUpHandler(handleKeyUp);

    /////////////////////////left , right, bottom, top,base, lid
    meshInitializeBox(&mesh0, -10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
    meshInitializeSphere(&mesh1, 5, 20, 20);

    sceneInitialize(&scen0, &ren, unif, tex, &mesh0,NULL,NULL);
    sceneInitialize(&scen1, &ren, unif, tex, &mesh1,NULL,NULL);
    sceneSetTexture(&scen1,&ren,0,&texture1);
    sceneSetUniform(&scen1,&ren,unif2);
    sceneAddChild(&scen0,&scen1);

    renLookAt(&ren, target, cam[2], cam[0], cam[1]);
    //renSetFrustum(&ren, renORTHOGRAPHIC, M_PI/6.0, 10.0, 10.0);
    renSetFrustum(&ren, renPERSPECTIVE, M_PI/6.0, 10.0, 10.0);
    //printf("pi is: %f\n",M_PI);

    draw();
    pixRun();


    texDestroy(tex[0]);
    meshDestroy(&mesh0);
    depthDestroy(&dep);
    texDestroy(tex[1]);
    meshDestroy(&mesh1);
    sceneDestroyRecursively(&scen0);



    return 0;
  }
}
