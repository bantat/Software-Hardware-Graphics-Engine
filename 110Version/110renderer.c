/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This file has a struct for rendering.
*/

typedef struct renRenderer renRenderer;

struct renRenderer {
  int unifDim;
  int texNum;
  int varyDim;
  int attrDim;
  void (*colorPixel)(renRenderer *, double[], texTexture *[], double[], double[]);
  void (*transformVertex)(renRenderer *, double[], double[], double[]);
  void (*updateUniform)(renRenderer *,double[],double[]);
  depthBuffer *depth;
};
