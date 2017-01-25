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

void hiddenRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {

  printf("start render\n");

  double xleft, yleft, xmid, ymid, xright, yright;

  xleft = a[renVARYX];
  yleft = a[renVARYY];

  xmid = b[renVARYX];
  ymid = b[renVARYY];

  xright = c[renVARYX];
  yright = c[renVARYY];

  //printf("[%f ; %f] , [%f ; %f] , [%f ; %f]\n",xleft,yleft,xmid,ymid,xright,yright);
  // this variable changes if the triangle is right angled so we dont devide by
  // zero
  int rght = 0;
  if (fabs(xleft - xmid) < 0.00001) {
    // straight line on the left side
    rght = 1;
  } else if (fabs(xmid - xright) < 0.00001) {
    // straight line on the right side
    rght = 2;
  }

  // Now draw the first half of the triangle
  // For every x coodinate between left most point to mid point
  //printf("rght = %d , ceil(xleft) = %d , floor(xmid) = %d\n", rght,(int)ceil(xleft), (int)floor(xmid));
  for (int i = (int)ceil(xleft); i <= (int)floor(xmid); i++) {
    double ylow, yhigh;
    int y_Top,y_Bottom;


    // If the mid point is above and (xleft is not inline with xmid or xleft)
    if (!(rght == 1)) {

      ylow = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      yhigh = yleft + (((ymid - yleft) / (xmid - xleft)) * (i - xleft));
      y_Bottom= ceil(fmin(ylow,yhigh));
      y_Top = floor(fmax(yhigh,ylow));

      // If the mid point is below and xleft is not inline with xmid
    } else if (!(rght == 1)) {
      yhigh = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      ylow = yleft + (((ymid - yleft) / (xmid - xleft)) * (i - xleft));
      y_Bottom= ceil(fmin(ylow,yhigh));
      y_Top = floor(fmax(yhigh,ylow));
    }


    // draw the triangle
    for (int j = y_Bottom; j <= y_Top; j++) {
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

      double rgb[3];
      ren->colorPixel(ren, unif, tex, vary, rgb);
      pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
    }
  }

  // from xmid to xright
  //printf("ceil(xmid) = %d, floor(xright) = %d\n", (int)ceil(xmid), (int)floor(xright));
  for (int i = (int)ceil(xmid); i <= (int)floor(xright); i++) {
    double ylow, yhigh;
    int y_Top,y_Bottom;

    // if mid point is above and xmid is not inline with xright
    if (!(rght == 2)) {
      ylow = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      yhigh = ymid + (((yright - ymid) / (xright - xmid)) * (i - xmid));
      y_Bottom= ceil(fmin(ylow,yhigh));
      y_Top = floor(fmax(yhigh,ylow));

      // if mid point is below and xmid is not inline with xright
    } else if (!(rght == 2)) {
      yhigh = yleft + (((yright - yleft) / (xright - xleft)) * (i - xleft));
      ylow = ymid + (((yright - ymid) / (xright - xmid)) * (i - xmid));
      y_Bottom= ceil(fmin(ylow,yhigh));
      y_Top = floor(fmax(yhigh,ylow));
    }
    printf("made it here\n");

    printf("y_bottom = %d, y_top = %d\n",y_Bottom, y_Top);

    for (int j = y_Bottom ; j <= y_Top; j++) {
      //printf("hanging in here\n");
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


      double rgb[3];
      ren->colorPixel(ren, unif, tex, vary, rgb);
      pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
    }
  }
}


void triRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {
      // Do the normalisation of the triangle by reassigning triangle coodinate
      // values and attributes

      //swap(a, c);
      if (a[0] > c[0]) {
        double *intM; intM = a; a = c; c = intM;
      }

      //swap(a, b);
      if (a[0] > b[0]) {
        double *intM; intM = a; a = b; b= intM;
      }

      //swap(b, c);
      if (b[0] > c[0]){
        double *intM; intM = b; b = c; c = intM;
      }

      hiddenRender(ren, unif, tex, a, b, c);
}
