/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This files includes the function that renders a triangle by rasterisation. It
also handles triangle coodinates
given in any order.
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
/*#include "040texture.c"


void vecCopy(int dim, double v[], double copy[]);
void vecAdd(int dim, double v[], double w[], double vPlusW[]);
void vecSubtract(int dim, double v[], double w[], double vMinusW[]);
void vecScale(int dim, double c, double w[], double cTimesW[]);

void mat22Print(double m[2][2]);
double mat22Invert(double m[2][2], double mInv[2][2]);
void mat22Multiply(double m[2][2], double v[2], double mTimesV[2]);
void mat22Columns(double col0[2], double col1[2], double m[2][2]);*/

// int texInitializeFile(texTexture *tex, const char *path);
// void texDestroy(texTexture *tex);
// void texSample(texTexture *tex, double s, double t);

/*
@function MAX
@param (double,double,double)
@purpose Finds the Maximum value of the three doubles given as parameters and
returns it.
*/
double MAX(a, b, c) {
  return ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c));
}

/*
@function triRender
@param (double a0,double a1,double b0,double b1,double c0,double c1,double
r,double g,double b), where
a0,...,b1 are coodinates of the triangle and r,g,b represent the color of the
triangle to be drawn.
@purpose This function first normalises whatever triangle coodinates it is
given. It then renders
the triangle according to the equations of lines. It also has some cotrol logic
for different triangle shapes
and situations.
*/
/*
void triRender(double a[2], double b[2], double c[2], double rgb[3],
               texTexture *tex, double alpha[2], double beta[2],
               double gamma[2]) {
*/

void triRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {

  // initialising the normalised triangle coodinates

  double xleft, yleft, xmid, ymid, xright, yright;
  double abgleft[renATTRDIMBOUND];
  double abgmid[renATTRDIMBOUND];
  double abgright[renATTRDIMBOUND];
  // Find the maximum x to figure out which is the right most coodinate
  double m = MAX(a[renATTRX], b[renATTRX], c[renATTRX]);

  // debugging print statement:

  // Do the normalisation of the triangle by reassigning triangle coodinate
  // values and attributes
  if (m == c[renATTRX]) {

    // This case c0 is the right most point so reassign
    xright = c[renATTRX];
    yright = c[renATTRY];
    vecCopy(renATTRDIMBOUND, c, abgright);

    // find the mid point and reassign
    if (b[renATTRX] == MAX(a[renATTRX], b[renATTRX], 0)) {
      xmid = b[renATTRX];
      ymid = b[renATTRY];
      xleft = a[renATTRX];
      yleft = a[renATTRY];
      vecCopy(renATTRDIMBOUND, b, abgmid);
      vecCopy(renATTRDIMBOUND, a, abgleft);
    } else {
      xmid = a[renATTRX];
      ymid = a[renATTRY];
      xleft = b[renATTRX];
      yleft = b[renATTRY];
      vecCopy(renATTRDIMBOUND, a, abgmid);
      vecCopy(renATTRDIMBOUND, b, abgleft);
    }

  } else if (m == b[renATTRX]) {
    // b0 is the right most point
    xright = b[renATTRX];
    yright = b[renATTRY];
    vecCopy(renATTRDIMBOUND, b, abgright);

    // finding the mid and the left most
    if (c[renATTRX] == MAX(a[renATTRX], c[renATTRX], 0)) {
      xmid = c[renATTRX];
      ymid = c[renATTRY];
      xleft = a[renATTRX];
      yleft = a[renATTRY];
      vecCopy(renATTRDIMBOUND, c, abgmid);
      vecCopy(renATTRDIMBOUND, a, abgleft);
    } else {
      xmid = a[renATTRX];
      ymid = a[renATTRY];
      xleft = c[renATTRX];
      yleft = c[renATTRY];
      vecCopy(renATTRDIMBOUND, a, abgmid);
      vecCopy(renATTRDIMBOUND, c, abgleft);
    }
  } else {
    // a0 is the right most x coodinate
    xright = a[renATTRX];
    yright = a[renATTRY];
    vecCopy(renATTRDIMBOUND, a, abgright);

    if (b[renATTRX] == MAX(b[renATTRX], c[renATTRX], 0)) {
      xmid = b[renATTRX];
      ymid = b[renATTRY];
      xleft = c[renATTRX];
      yleft = c[renATTRY];
      vecCopy(renATTRDIMBOUND, b, abgmid);
      vecCopy(renATTRDIMBOUND, c, abgleft);
    } else {
      xmid = c[renATTRX];
      ymid = c[renATTRY];
      xleft = b[renATTRX];
      yleft = b[renATTRY];
      vecCopy(renATTRDIMBOUND, c, abgmid);
      vecCopy(renATTRDIMBOUND, b, abgleft);
    }
  }

  // figure out if the mid point is above line (left most) ---> (right most),
  // teslls us the shape of triangle
  int shp = (ymid < MAX(yleft, yright, 0) ? 1 : 2);

  // this variable changes if the triangle is right angled so we dont devide by
  // zero
  int rght = 0;
  // Figuring out if the triangle is right angled
  if (xleft == xmid) {
    // straight line on the left side
    rght = 1;
  } else if (xmid == xright) {
    // straight line on the right side
    rght = 2;
  }

  // printf("[r=%f,b=%f,g=%f]\n",rgb[0],rgb[1],rgb[2]);

  // check which one is the middle one
  // debug and check if the the coodinates are correctly normalised, shape is
  // correct and equal x-coodinates detected
  // printf("xleft = %f, yleft= %f, xmid = %f,ymid= %f, xright = %f,yright =
  // %f,shape = %d,right angled = %d\n"
  //	,xleft,yleft,xmid,ymid,xright,yright,shp,rght);

  /*refactoring variables so that they match the converntional names we have
  been using in class*/
  //a[renATTRX] = xleft;
  //a[renATTRY] = yleft;
  vecCopy(renATTRDIMBOUND, abgleft, a);
  //b[renATTRX] = xmid;
  //b[renATTRY] = ymid;
  vecCopy(renATTRDIMBOUND, abgmid, b);
  //c[renATTRX] = xright;
  //c[renATTRY] = yright;
  vecCopy(renATTRDIMBOUND, abgright, c);

  // Now draw the first half of the triangle
  // For every x coodinate between left most point to mid point
  for (int i = ceil(xleft); i <= floor(xmid); i++) {
    double ylow, yhigh;

    // If the mid point is above and (xleft is not inline with xmid or xleft)
    if (shp == 2 && !(rght == 1)) {
      ylow = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      yhigh = yleft + (((ymid - yleft) / (xmid - xleft)) * (i - xleft));

      // If the mid point is below and xleft is not inline with xmid
    } else if (shp == 1 && !(rght == 1)) {
      yhigh = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      ylow = yleft + (((ymid - yleft) / (xmid - xleft)) * (i - xleft));
    }

    // draw the triangle
    for (int j = ceil(ylow); j < floor(yhigh); j++) {
      double bminusa[2], cminusa[2], xminusa[2], m[2][2], mInv[2][2], pq[2],
          x[2];
      x[0] = i;
      x[1] = j;
      // printf("[x= %d, y= %d]\n",i,j);
      vecSubtract(2, b, a, bminusa);
      vecSubtract(2, c, a, cminusa);
      vecSubtract(2, x, a, xminusa);

      mat22Columns(bminusa, cminusa, m);
      mat22Columns(bminusa, cminusa, mInv);
      mat22Invert(m, mInv);

      mat22Multiply(mInv, xminusa, pq);
      double p = pq[0];
      double q = pq[1];

      double betaminusalpha[renATTRDIMBOUND], gammaminusalpha[renATTRDIMBOUND], attr[renATTRDIMBOUND];

      vecSubtract(renATTRDIMBOUND, b, a, betaminusalpha);
      vecSubtract(renATTRDIMBOUND, c, a, gammaminusalpha);

      double pvec[renATTRDIMBOUND], qvec[renATTRDIMBOUND], pplusqvec[renATTRDIMBOUND];

      vecScale(renATTRDIMBOUND, p, betaminusalpha, pvec);
      vecScale(renATTRDIMBOUND, q, gammaminusalpha, qvec);

      vecAdd(renATTRDIMBOUND, pvec, qvec, pplusqvec);
      vecAdd(renATTRDIMBOUND, pplusqvec, a, attr);

      //texSample(tex, chi[0], chi[1]);

      double rgb[3];

      colorPixel(ren, unif, tex, attr, rgb);
      pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);

      /*
      // printf("[r=%f,b=%f,g=%f]\n",rgb[0],rgb[1],rgb[2]);
      pixSetRGB(i, j, tex->sample[0] * rgb[0], tex->sample[1] * rgb[1],
                tex->sample[2] * rgb[2]);
      */
    }
  }

  // from xmid to xright
  for (int i = floor(xmid); i <= (floor(xright)); i++) {
    double ylow, yhigh;

    // if mid point is above and xmid is not inline with xright
    if (shp == 2 && !(rght == 2)) {
      ylow = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      yhigh = ymid + (((yright - ymid) / (xright - xmid)) * (i - xmid));
      // if mid point is below and xmid is not inline with xright
    } else if (shp == 1 && !(rght == 2)) {
      yhigh = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      ylow = ymid + (((yright - ymid) / (xright - xmid)) * (i - xmid));
    }

    for (int j = ceil(ylow); j <= floor(yhigh); j++) {
      double bminusa[2], cminusa[2], xminusa[2], m[2][2], mInv[2][2], pq[2],
          x[2];
      x[0] = i;
      x[1] = j;

      vecSubtract(2, b, a, bminusa);
      vecSubtract(2, c, a, cminusa);
      vecSubtract(2, x, a, xminusa);

      mat22Columns(bminusa, cminusa, m);
      mat22Columns(bminusa, cminusa, mInv);
      mat22Invert(m, mInv);

      mat22Multiply(mInv, xminusa, pq);
      double p = pq[0];
      double q = pq[1];

      double betaminusalpha[renATTRDIMBOUND], gammaminusalpha[renATTRDIMBOUND], attr[renATTRDIMBOUND];

      vecSubtract(renATTRDIMBOUND, b, a, betaminusalpha);
      vecSubtract(renATTRDIMBOUND, c, a, gammaminusalpha);

      double pvec[renATTRDIMBOUND], qvec[renATTRDIMBOUND], pplusqvec[renATTRDIMBOUND];

      vecScale(renATTRDIMBOUND, p, betaminusalpha, pvec);
      vecScale(renATTRDIMBOUND, q, gammaminusalpha, qvec);

      vecAdd(renATTRDIMBOUND, pvec, qvec, pplusqvec);
      vecAdd(renATTRDIMBOUND, pplusqvec, a, attr);

      /*

      // printf("[%f,%f,%f]\n",chi[0],chi[1],chi[2]);
      texSample(tex, chi[0], chi[1]);
      pixSetRGB(i, j, tex->sample[0] * rgb[0], tex->sample[1] * rgb[1],
                tex->sample[2] * rgb[2]);
      */

      double rgb[3];

      colorPixel(ren, unif, tex, attr, rgb);
      pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
    }
  }
}
