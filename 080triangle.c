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
  printf("infucntion [%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
  double xleft, yleft, xmid, ymid, xright, yright;

  xleft = a[renVARYX];
  yleft = a[renVARYY];

  xmid = b[renVARYX];
  ymid = b[renVARYY];

  xright = c[renVARYX];
  yright = c[renVARYY];

  int shp = 1;

  // figure out if the mid point is above line (left most) ---> (right most),
  // teslls us the shape of triangle
  if (ymid <= yright && ymid <= yleft) {
    shp = 1;
  } else {
    shp = 2;
  }

  //printf("shp = %d\n", shp);

  //int shp = (ymid < MAX(yleft, yright, 0) ? 1 : 2);

  // this variable changes if the triangle is right angled so we dont devide by
  // zero
  int rght = 0;
  // Figuring out if the triangle is right angled
  /*
  printf("xleft - xmid: %f\n", xleft-xmid);
  printf("xmid - xright: %f\n", xmid-xright);
  if ((xleft - xmid) < 1) {
    printf("xleft: %f, xmid: %f\n", xleft, xmid);
  }
  if ((xmid - xright) < 1) {
    printf("xmid: %f, xright: %f\n", xmid, xright);
  }
*/


  if (fabs(xleft - xmid) < 0.00001) {
    // straight line on the left side
    //printf("Straight line left\n");
    rght = 1;
  } else if (fabs(xmid - xright) < 0.00001) {
    // straight line on the right side
    //printf("Straight line right\n");
    rght = 2;
  }

  //printf("rght = %d, [%f,%f] ,[%f,%f] ,[%f,%f]\n", rght,xleft,yleft,xmid,ymid,xright,yright);

  /*refactoring variables so that they match the converntional names we have
  been using in class*/
  // vecCopy(ren->varyDim, abgleft, a);
  // vecCopy(ren->varyDim, abgmid, b);
  // vecCopy(ren->varyDim, abgright, c);


  // Now draw the first half of the triangle
  // For every x coodinate between left most point to mid point
  printf("ceil(xleft) = %d, floor(xmid) = %d\n", (int)ceil(xleft), (int)floor(xmid));
  for (int i = (int)ceil(xleft); i <= (int)floor(xmid); i++) {
    double ylow, yhigh;

    //printf("Drawn left side\n");
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
    for (int j = ceil(ylow); j <= floor(yhigh); j++) {
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
  printf("ceil(xmid) = %d, floor(xright) = %d\n", (int)ceil(xmid), (int)floor(xright));
  for (int i = (int)ceil(xmid); i <= (int)floor(xright); i++) {
    double ylow, yhigh;
    //printf("Drawn right side\n");
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


void triRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {


      //printf("got into tri\n");
      // Do the normalisation of the triangle by reassigning triangle coodinate
      // values and attributes
      //hiddenRender(ren, unif, tex, a, b, c);


      //printf("before [%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);

      if (a[0] > c[0]) {
        //swap(a, c);
        double *intM;
        intM = a;
        a = c;
        c = intM;
        //printf("before [%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);


      }

      if (a[0] > b[0]) {
        double *intM;
        //swap(a, b);
        //printf("before [%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
        intM = a;
        a = b;
        b= intM;
      }

      if (b[0] > c[0]){
        double *intM;
        //swap(b, c);
        intM = b;
        b = c;
        c = intM;
      }

      //printf("after [%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, a, b, c);
/*
  if (a[0] <= b[0] && a[0] <= c[0]) {
    if (b[0] <= c[0]) {
      printf("render trinangle1\n");
      printf("[%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, a, b, c);
    } else {
      printf("render trinangle2\n");
      printf("[%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, a, c, b);
    }
  }
  else if (b[0] <= a[0] && b[0] <= c[0]) {
    if (a[0] <= c[0]) {
      printf("render trinangle3\n");
      printf("[%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, b, a, c);
    } else {
      printf("render trinangle4\n");
      printf("[%.20lf,%.20lf] [%.20lf,%.20lf] [%.20lf,%.20lf]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, b, c, a);
    }
  }
  else {
    if (a[0] <= b[0]) {
      printf("render trinangle5\n");
      printf("[%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, c, a, b);
    }
    else {
      printf("render trinangle6\n");
      printf("[%f,%f] [%f,%f] [%f, %f]\n", a[0],a[1],b[0],b[1],c[0],c[1]);
      hiddenRender(ren, unif, tex, c, b, a);
    }
  }*/
}
