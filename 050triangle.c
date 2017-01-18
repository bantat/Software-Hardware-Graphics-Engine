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

void triRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {

  // initialising the normalised triangle coodinates

  double xleft, yleft, xmid, ymid, xright, yright;
  double abgleft[ren->varyDim];
  double abgmid[ren->varyDim];
  double abgright[ren->varyDim];
  // Find the maximum x to figure out which is the right most coodinate
  double m = MAX(a[renVARYX], b[renVARYX], c[renVARYX]);

  // debugging print statement:

  // Do the normalisation of the triangle by reassigning triangle coodinate
  // values and attributes
  if (m == c[renVARYX]) {

    // This case c0 is the right most point so reassign
    xright = c[renVARYX];
    yright = c[renVARYY];
    vecCopy(ren->varyDim, c, abgright);

    // find the mid point and reassign
    if (b[renVARYX] == MAX(a[renVARYX], b[renVARYX], 0)) {
      xmid = b[renVARYX];
      ymid = b[renVARYY];
      xleft = a[renVARYX];
      yleft = a[renVARYY];
      vecCopy(ren->varyDim, b, abgmid);
      vecCopy(ren->varyDim, a, abgleft);
    } else {
      xmid = a[renVARYX];
      ymid = a[renVARYY];
      xleft = b[renVARYX];
      yleft = b[renVARYY];
      vecCopy(ren->varyDim, a, abgmid);
      vecCopy(ren->varyDim, b, abgleft);
    }

  } else if (m == b[renVARYX]) {
    // b0 is the right most point
    xright = b[renVARYX];
    yright = b[renVARYY];
    vecCopy(ren->varyDim, b, abgright);

    // finding the mid and the left most
    if (c[renVARYX] == MAX(a[renVARYX], c[renVARYX], 0)) {
      xmid = c[renVARYX];
      ymid = c[renVARYY];
      xleft = a[renVARYX];
      yleft = a[renVARYY];
      vecCopy(ren->varyDim, c, abgmid);
      vecCopy(ren->varyDim, a, abgleft);
    } else {
      xmid = a[renVARYX];
      ymid = a[renVARYY];
      xleft = c[renVARYX];
      yleft = c[renVARYY];
      vecCopy(ren->varyDim, a, abgmid);
      vecCopy(ren->varyDim, c, abgleft);
    }
  } else {
    // a0 is the right most x coodinate
    xright = a[renVARYX];
    yright = a[renVARYY];
    vecCopy(ren->varyDim, a, abgright);

    if (b[renVARYX] == MAX(b[renVARYX], c[renVARYX], 0)) {
      xmid = b[renVARYX];
      ymid = b[renVARYY];
      xleft = c[renVARYX];
      yleft = c[renVARYY];
      vecCopy(ren->varyDim, b, abgmid);
      vecCopy(ren->varyDim, c, abgleft);
    } else {
      xmid = c[renVARYX];
      ymid = c[renVARYY];
      xleft = b[renVARYX];
      yleft = b[renVARYY];
      vecCopy(ren->varyDim, c, abgmid);
      vecCopy(ren->varyDim, b, abgleft);
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
  vecCopy(ren->varyDim, abgleft, a);
  vecCopy(ren->varyDim, abgmid, b);
  vecCopy(ren->varyDim, abgright, c);
/////////////////////////////////////////////////////ACTUAL TRIRENDER///////////
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

      double betaminusalpha[renVARYDIMBOUND], gammaminusalpha[renVARYDIMBOUND], vary[renVARYDIMBOUND];

      vecSubtract(ren->varyDim, b, a, betaminusalpha);
      vecSubtract(ren->varyDim, c, a, gammaminusalpha);

      double pvec[renVARYDIMBOUND], qvec[renVARYDIMBOUND], pplusqvec[renVARYDIMBOUND];

      vecScale(ren->varyDim, p, betaminusalpha, pvec);
      vecScale(ren->varyDim, q, gammaminusalpha, qvec);

      vecAdd(ren->varyDim, pvec, qvec, pplusqvec);
      vecAdd(ren->varyDim, pplusqvec, a, vary);

      //texSample(tex, chi[0], chi[1]);

      double rgb[3];

      colorPixel(ren, unif, tex, vary, rgb);
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

    // Where is rght == 1...

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

      double betaminusalpha[renVARYDIMBOUND], gammaminusalpha[renVARYDIMBOUND], vary[renVARYDIMBOUND];

      vecSubtract(ren->varyDim, b, a, betaminusalpha);
      vecSubtract(ren->varyDim, c, a, gammaminusalpha);

      double pvec[renVARYDIMBOUND], qvec[renVARYDIMBOUND], pplusqvec[renVARYDIMBOUND];

      vecScale(ren->varyDim, p, betaminusalpha, pvec);
      vecScale(ren->varyDim, q, gammaminusalpha, qvec);

      vecAdd(ren->varyDim, pvec, qvec, pplusqvec);
      vecAdd(ren->varyDim, pplusqvec, a, vary);

      /*

      // printf("[%f,%f,%f]\n",chi[0],chi[1],chi[2]);
      texSample(tex, chi[0], chi[1]);
      pixSetRGB(i, j, tex->sample[0] * rgb[0], tex->sample[1] * rgb[1],
                tex->sample[2] * rgb[2]);
      */

      double rgb[3];

      colorPixel(ren, unif, tex, vary, rgb);
      pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
    }
  }
}
