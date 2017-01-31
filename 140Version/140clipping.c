/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This file has functions for clipping.
*/

void clip_one(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[], int clip_vert) {
  // test

}

void clip_two(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[], int clip_vert1, int clip_vert2) {
  // test

}

void hiddenClipRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
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
  /// skip this if it dont work ///

  int a_clip = 0;
  int b_clip = 0;
  int c_clip = 0;

  if (a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW]) {
    a_clip = 1;
  }

  if (b[renVARYW] <= 0 || b[renVARYZ] > b[renVARYW]) {
    b_clip = 1;
  }

  if (c[renVARYW] <= 0 || c[renVARYZ] > c[renVARYW]) {
    c_clip = 1;
  }

  if (a_clip == 1 && b_clip == 1 && c_clip == 1) {
    return;
  } else if (a_clip == 0 && b_clip == 0 && c_clip == 0) {
    triRender(ren, unif, tex, a, b, c);
  } else {
    if (a_clip == 1 && b_clip == 0 && c_clip == 0) {
      clip_one(ren, unif, tex, a, b, c, 1);
    } else if (a_clip == 1 && b_clip == 1 && c_clip == 0) {
      clip_two(ren, unif, tex, a, b, c, 1, 2);
    } else if (a_clip == 1 && b_clip == 0 && c_clip == 1) {
      clip_two(ren, unif, tex, a, b, c, 1, 3);
    } else if (a_clip == 0 && b_clip == 1 && c_clip == 0) {
      clip_one(ren, unif, tex, a, b, c, 2);
    } else if (a_clip == 0 && b_clip == 1 && c_clip == 1) {
      clip_two(ren, unif, tex, a, b, c, 2, 3);
    } else if (a_clip == 0 && b_clip == 0 && c_clip == 1) {
      clip_one(ren, unif, tex, a, b, c, 3);
    }
  }

  /// skip this if it dont work ///
  //triRender(ren, unif, tex, a, b, c);
}

void clipRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {
  // Do the normalisation of the triangle by reassigning triangle coodinate
  // values and attributes

  double scaleVec[renVARYDIMBOUND];

  double aTrans[renVARYDIMBOUND];
  double bTrans[renVARYDIMBOUND];
  double cTrans[renVARYDIMBOUND];

  vecScale(ren->attrDim, 1.0/a[renVARYW], a, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, aTrans);
  aTrans[renVARYS] = a[renVARYS];
  aTrans[renVARYT] = a[renVARYT];

  vecScale(ren->attrDim, 1.0/b[renVARYW], b, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, bTrans);
  bTrans[renVARYS] = b[renVARYS];
  bTrans[renVARYT] = b[renVARYT];

  vecScale(ren->attrDim, 1.0/c[renVARYW], c, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, cTrans);
  cTrans[renVARYS] = c[renVARYS];
  cTrans[renVARYT] = c[renVARYT];

  if (aTrans[0] <= bTrans[0] && aTrans[0] <= cTrans[0]) {
    hiddenClipRender(ren, unif, tex, aTrans, bTrans, cTrans);
  }

  else if (bTrans[0] <= aTrans[0] && bTrans[0] <= cTrans[0]) {
    hiddenClipRender(ren, unif, tex, bTrans, cTrans, aTrans);
  }

  else if (cTrans[0] <= bTrans[0] && cTrans[0] <= aTrans[0]) {
    hiddenClipRender(ren, unif, tex, cTrans, aTrans, bTrans);
  }
}
