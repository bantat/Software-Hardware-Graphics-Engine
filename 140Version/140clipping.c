/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This file has functions for clipping.
*/

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
    hiddenRender(ren, unif, tex, aTrans, bTrans, cTrans);
  }

  else if (bTrans[0] <= aTrans[0] && bTrans[0] <= cTrans[0]) {
    hiddenRender(ren, unif, tex, bTrans, cTrans, aTrans);
  }

  else if (cTrans[0] <= bTrans[0] && cTrans[0] <= aTrans[0]) {
    hiddenRender(ren, unif, tex, cTrans, aTrans, bTrans);
  }
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

  double vert_a[renVARYDIMBOUND], vert_b[renVARYDIMBOUND], vert_c[renVARYDIMBOUND];

  vecCopy(ren->varyDim, a, vert_a);

  if (b[0] <= c[0]) {
    vecCopy(ren->varyDim, b, vert_b);
    vecCopy(ren->varyDim, c, vert_c);
  } else {
    vecCopy(ren->varyDim, c, vert_b);
    vecCopy(ren->varyDim, b, vert_c);
  }



}
