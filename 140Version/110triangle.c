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

  //do shiftIsDown
  double bminusa[2], cminusa[2], m[2][2], mInv[2][2], pq[2];
  vecSubtract(2, b, a, bminusa);
  vecSubtract(2, c, a, cminusa);
  mat22Columns(bminusa, cminusa, m);
  mat22Columns(bminusa, cminusa, mInv);
  if (mat22Invert(m, mInv) < 0.0) {
    return;
  }

  double xleft, yleft, xmid, ymid, xright, yright;

  xleft = a[renVARYX];
  yleft = a[renVARYY];

  if (b[0] <= c[0]) {
    xmid = b[renVARYX];
    ymid = b[renVARYY];

    xright = c[renVARYX];
    yright = c[renVARYY];
  } else {
    xmid = c[renVARYX];
    ymid = c[renVARYY];

    xright = b[renVARYX];
    yright = b[renVARYY];
  }

  // Now draw the first half of the triangle
  // For every x coodinate between left most point to mid point
  //printf("rght = %d , ceil(xleft) = %d , floor(xmid) = %d\n", rght,(int)ceil(xleft), (int)floor(xmid));
  for (int i = (int)ceil(xleft); i <= (int)floor(xmid); i++) {
    double ylow, yhigh;
    int y_Top,y_Bottom;

    ylow = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
    yhigh = yleft + (((ymid - yleft) / (xmid - xleft)) * (i - xleft));
    y_Bottom= ceil(fmin(ylow,yhigh));
    y_Top = floor(fmax(yhigh,ylow));


    // draw the triangle
    for (int j = y_Bottom; j <= y_Top; j++) {
      double xminusa[2], x[2];
      x[0] = i;
      x[1] = j;

      vecSubtract(2, x, a, xminusa);

      mat221Multiply(mInv, xminusa, pq);
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

      double rgbz[4];
      if(vary[renVARYZ] > depthGetZ(ren->depth,i,j)){
        ren->colorPixel(ren, unif, tex, vary, rgbz);
        pixSetRGB(i, j, rgbz[0], rgbz[1], rgbz[2]);
        depthSetZ(ren->depth,i,j,vary[renVARYZ]);
      }
    }
}

  // from xmid to xright
  //printf("ceil(xmid) = %d, floor(xright) = %d\n", (int)ceil(xmid), (int)floor(xright));
  for (int i = (int)ceil(xmid); i <= (int)floor(xright); i++) {
    double ylow, yhigh;
    int y_Top,y_Bottom;

    ylow = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
    yhigh = ymid + (((yright - ymid) / (xright - xmid)) * (i - xmid));
    y_Bottom= ceil(fmin(ylow,yhigh));
    y_Top = floor(fmax(yhigh,ylow));

    double xminusa[2], x[2];

    for (int j = y_Bottom ; j <= y_Top; j++) {
      x[0] = i;
      x[1] = j;

      vecSubtract(2, x, a, xminusa);

      mat221Multiply(mInv, xminusa, pq);
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


      double rgbz[4];
      //printf("[%f,%f,%f]\n", vary[0], vary[1], vary[2]);
      if(vary[renVARYZ] > depthGetZ(ren->depth,i,j)){
        ren->colorPixel(ren, unif, tex, vary, rgbz);
        pixSetRGB(i, j, rgbz[0], rgbz[1], rgbz[2]);
        depthSetZ(ren->depth,i,j,vary[renVARYZ]);
      }

    }
  }
}


void hiddenRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {
      // Do the normalisation of the triangle by reassigning triangle coodinate
      // values and attributes

      if (a[0] <= b[0] && a[0] <= c[0]) {
        hiddenRender(ren, unif, tex, a, b, c);
      }

      else if (b[0] <= a[0] && b[0] <= c[0]) {
        hiddenRender(ren, unif, tex, b, c, a);
      }

      else if (c[0] <= b[0] && c[0] <= a[0]) {
        hiddenRender(ren, unif, tex, c, a, b);
      }
}
