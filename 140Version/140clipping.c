/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This file has functions for clipping.
*/

void doViewPort(renRenderer *ren, double vert[]) {
  double scaleVec[renVARYDIMBOUND];
  double test[renVARYDIMBOUND];
    //printf("******\n rendering1... [%f,%f,%f,%f]\n",vert[0],vert[1],vert[2],vert[3]);
  vecScale(ren->varyDim, 1.0/vert[renVARYW], vert, scaleVec);
//printf("rendering2... [%f,%f,%f,%f]\n",scaleVec[0],scaleVec[1],scaleVec[2],scaleVec[3]);
  mat441Multiply(ren->viewport, scaleVec, vert);
  printf("rendering2... [%f,%f,%f,%f]\n",vert[0],vert[1],vert[2],vert[3]);
}

void clip_one(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[], int clip_vert) {

  //call doViewPort for each vertex
  double sub_vec[renVARYDIMBOUND], add_vec[renVARYDIMBOUND], t;
  if (clip_vert == 1) {
    t = (a[3]-a[2])/(a[3]-a[2]+b[2]-b[3]);
    double new_aL[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, b, a, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, a, add_vec, new_aL);

    t = (a[3]-a[2])/(a[3]-a[2]+c[2]-c[3]);
    double new_aR[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, c, a, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, a, add_vec, new_aR);

    doViewPort(ren, new_aL);
    doViewPort(ren, new_aR);
    doViewPort(ren, b);
    doViewPort(ren, c);

    triRender(ren, unif, tex, new_aL, b, new_aR);
    triRender(ren, unif, tex, new_aR, b, c);
  } else if (clip_vert == 2) {
    t = (b[3]-b[2])/(b[3]-b[2]+c[2]-c[3]);
    double new_bL[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, c, b, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, b, add_vec, new_bL);

    t = (b[3]-b[2])/(b[3]-b[2]+a[2]-a[3]);
    double new_bR[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, a, b, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, b, add_vec, new_bR);

    doViewPort(ren, new_bL);
    doViewPort(ren, new_bR);
    doViewPort(ren, a);
    doViewPort(ren, c);

    triRender(ren, unif, tex, new_bL, c, new_bR);
    triRender(ren, unif, tex, new_bR, c, a);
  } else if (clip_vert == 3) {
    t = (c[3]-c[2])/(c[3]-c[2]+a[2]-a[3]);
    double new_cL[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, a, c, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, c, add_vec, new_cL);

    t = (c[3]-c[2])/(c[3]-c[2]+b[2]-b[3]);
    double new_cR[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, b, c, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, c, add_vec, new_cR);

    doViewPort(ren, new_cL);
    doViewPort(ren, new_cR);
    doViewPort(ren, a);
    doViewPort(ren, b);

    triRender(ren, unif, tex, new_cL, a, new_cR);
    triRender(ren, unif, tex, new_cR, a, b);
  }
}

void clip_two(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[], int clip_vert1, int clip_vert2) {
  double sub_vec[renVARYDIMBOUND], add_vec[renVARYDIMBOUND], t;

  if (clip_vert1 == 1 && clip_vert2 == 2) {
    t = (a[3]-a[2])/(a[3]-a[2]+c[2]-c[3]);
    double new_a[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, c, a, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, a, add_vec, new_a);

    t = (b[3]-b[2])/(b[3]-b[2]+c[2]-c[3]);
    double new_b[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, c, a, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, b, add_vec, new_b);

    doViewPort(ren, new_a);
    doViewPort(ren, new_b);
    doViewPort(ren, c);

    triRender(ren, unif, tex, new_a, new_b, c);
  }
  else if (clip_vert1 == 1 && clip_vert2 == 3) {

    t = (a[3]-a[2])/(a[3]-a[2]+b[2]-b[3]);
    double new_a[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, b, a, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, a, add_vec, new_a);

    t = (c[3]-c[2])/(c[3]-c[2]+b[2]-b[3]);
    double new_c[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, b, c, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, c, add_vec, new_c);

    doViewPort(ren, new_a);
    doViewPort(ren, new_c);
    doViewPort(ren, b);

    triRender(ren, unif, tex, new_a, b, new_c);

  }
  else if (clip_vert1 == 2 && clip_vert2 == 3) {

    t = (b[3]-b[2])/(b[3]-b[2]+a[2]-a[3]);
    double new_b[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, a, b, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, b, add_vec, new_b);

    t = (c[3]-c[2])/(c[3]-c[2]+a[2]-a[3]);
    double new_c[renVARYDIMBOUND];
    vecSubtract(ren->varyDim, a, c, sub_vec);
    vecScale(ren->varyDim, t, sub_vec, add_vec);
    vecAdd(ren->varyDim, c, add_vec, new_c);

    doViewPort(ren, a);
    doViewPort(ren, new_c);
    doViewPort(ren, new_b);

    triRender(ren, unif, tex, a, new_b, new_c);

  }
}

void clipRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
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

  //doViewPort(ren, a);
  //doViewPort(ren, b);
  //doViewPort(ren, c);
  double scaleVec[renVARYDIMBOUND];
  double view_a[renVARYDIMBOUND], view_b[renVARYDIMBOUND], view_c[renVARYDIMBOUND];

  vecScale(ren->varyDim, 1.0/a[renVARYW], a, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, view_a);
  vecScale(ren->varyDim, 1.0/b[renVARYW], b, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, view_b);
  vecScale(ren->varyDim, 1.0/c[renVARYW], c, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, view_c);

  view_a[renVARYS] = a[renVARYS];
  view_a[renVARYT] = a[renVARYT];
  view_b[renVARYS] = b[renVARYS];
  view_b[renVARYT] = b[renVARYT];
  view_c[renVARYS] = c[renVARYS];
  view_c[renVARYT] = c[renVARYT];

  triRender(ren,unif,tex,view_a,view_b,view_c);

  /// skip this if it don't work ///
/*
  int a_clip = 0;
  int b_clip = 0;
  int c_clip = 0;

  if (a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW]) a_clip = 1;
  if (b[renVARYW] <= 0 || b[renVARYZ] > b[renVARYW]) b_clip = 1;
  if (c[renVARYW] <= 0 || c[renVARYZ] > c[renVARYW]) c_clip = 1;


  if (a_clip == 1 && b_clip == 1 && c_clip == 1) {
    printf("clipped triangle\n");
    return;
  } else if (a_clip == 0 && b_clip == 0 && c_clip == 0) {
    //call doViewPort for each vertex
    printf("not clipped\n");
    doViewPort(ren, a);
    doViewPort(ren, b);
    doViewPort(ren, c);
    printf("rendering... [%f,%f,%f] [%f,%f,%f] [%f,%f,%f]\n",a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2]);
    triRender(ren, unif, tex, a, b, c);
  } else {
    if (a_clip == 1 && b_clip == 0 && c_clip == 0) {
      printf("1\n");
      clip_one(ren, unif, tex, a, b, c, 1);
    } else if (a_clip == 1 && b_clip == 1 && c_clip == 0) {
      printf("2\n");
      clip_two(ren, unif, tex, a, b, c, 1, 2);
    } else if (a_clip == 1 && b_clip == 0 && c_clip == 1) {
      printf("3\n");
      clip_two(ren, unif, tex, a, b, c, 1, 3);
    } else if (a_clip == 0 && b_clip == 1 && c_clip == 0) {
      printf("4\n");
      clip_one(ren, unif, tex, a, b, c, 2);
    } else if (a_clip == 0 && b_clip == 1 && c_clip == 1) {
      printf("5\n");
      clip_two(ren, unif, tex, a, b, c, 2, 3);
    } else if (a_clip == 0 && b_clip == 0 && c_clip == 1) {
      printf("6\n");
      clip_one(ren, unif, tex, a, b, c, 3);
    }
  }
  */
}
