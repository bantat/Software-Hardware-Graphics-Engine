/*
@ Author:  Sabastian Mugazambi & Tore Banta
@ Date: 01/07/2017
This file has functions for clipping.
*/

void doViewPort(renRenderer *ren, double ogvert[] , double transVert[]){

  double scaleVec[renVARYDIMBOUND];
  vecScale(ren->varyDim, 1.0/ogvert[renVARYW], ogvert, scaleVec);
  mat441Multiply(ren->viewport, scaleVec, transVert);

  vecCopy(ren->varyDim-4, &ogvert[renVARYS], &transVert[renVARYS]);


}

void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[]){

  double sub_vec[renVARYDIMBOUND], add_vec[renVARYDIMBOUND];
  double t = (clipped[3]-clipped[2])/(clipped[3]-clipped[2]+notClipped[2]-notClipped[3]);

  vecSubtract(ren->varyDim, notClipped, clipped, sub_vec);
  vecScale(ren->varyDim, t, sub_vec, add_vec);
  vecAdd(ren->varyDim, clipped, add_vec,finalVec);
}

void clip_one(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[], int clip_vert) {

  //call doViewPort for each vertex
  double sub_vec[renVARYDIMBOUND], add_vec[renVARYDIMBOUND], t;
  double scaleVec[renVARYDIMBOUND];
  double view_a[renVARYDIMBOUND], view_b[renVARYDIMBOUND], view_c[renVARYDIMBOUND];
  double view_L[renVARYDIMBOUND], view_R[renVARYDIMBOUND];

  if (clip_vert == 1) {

    double new_aL[renVARYDIMBOUND], new_aR[renVARYDIMBOUND];
    //void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[])
    findNewVert(ren,a,b,new_aL);
    findNewVert(ren,a,c,new_aR);

    doViewPort(ren,new_aL,view_L);
    doViewPort(ren,b,view_b);
    doViewPort(ren,new_aR,view_R);
    doViewPort(ren,c,view_c);

    triRender(ren, unif, tex, view_L, view_b, view_R);
    triRender(ren, unif, tex, view_R, view_b, view_c);

  } else if (clip_vert == 2) {

    double new_bL[renVARYDIMBOUND], new_bR[renVARYDIMBOUND];
    //void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[])
    findNewVert(ren,b,c,new_bL);
    findNewVert(ren,b,a,new_bR);

    doViewPort(ren,new_bL,view_L);
    doViewPort(ren,a,view_a);
    doViewPort(ren,new_bR,view_R);
    doViewPort(ren,c,view_c);

    triRender(ren, unif, tex, view_L, view_c, view_R);
    triRender(ren, unif, tex, view_R, view_c, view_a);

  } else if (clip_vert == 3) {

    double new_cL[renVARYDIMBOUND], new_cR[renVARYDIMBOUND];
    //void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[])
    findNewVert(ren,c,a,new_cL);
    findNewVert(ren,c,b,new_cR);

    doViewPort(ren,new_cL,view_L);
    doViewPort(ren,b,view_b);
    doViewPort(ren,new_cR,view_R);
    doViewPort(ren,a,view_a);

    triRender(ren, unif, tex, new_cL, a, new_cR);
    triRender(ren, unif, tex, new_cR, a, b);

  }
}

void clip_two(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[], int clip_vert1, int clip_vert2) {
  double view_a[renVARYDIMBOUND], view_b[renVARYDIMBOUND], view_c[renVARYDIMBOUND];

  if (clip_vert1 == 1 && clip_vert2 == 2) {

    double new_a[renVARYDIMBOUND], new_b[renVARYDIMBOUND];
    //void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[])
    findNewVert(ren,a,c,new_a);
    findNewVert(ren,a,b,new_b);

    doViewPort(ren,new_a,view_a);
    doViewPort(ren,new_b,view_b);
    doViewPort(ren,c,view_c);

    triRender(ren, unif, tex, view_a, view_b, view_c);
  }
  else if (clip_vert1 == 1 && clip_vert2 == 3) {

    double new_a[renVARYDIMBOUND], new_c[renVARYDIMBOUND];
    //void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[])
    findNewVert(ren,a,b,new_a);
    findNewVert(ren,c,b,new_c);

    doViewPort(ren,new_a,view_a);
    doViewPort(ren,b,view_b);
    doViewPort(ren,new_c,view_c);

    triRender(ren, unif, tex, view_a, view_b, view_c);

  }
  else if (clip_vert1 == 2 && clip_vert2 == 3) {

    double new_b[renVARYDIMBOUND], new_c[renVARYDIMBOUND];
    //void findNewVert(renRenderer *ren, double clipped[], double notClipped[], double finalVec[])
    findNewVert(ren,b,a,new_b);
    findNewVert(ren,c,a,new_c);

    doViewPort(ren,a,view_a);
    doViewPort(ren,new_b,view_b);
    doViewPort(ren,new_c,view_c);

    triRender(ren, unif, tex, view_a, view_b, view_c);

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


  int a_clip = 0;
  int b_clip = 0;
  int c_clip = 0;

  if (a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW]) a_clip = 1;
  if (b[renVARYW] <= 0 || b[renVARYZ] > b[renVARYW]) b_clip = 1;
  if (c[renVARYW] <= 0 || c[renVARYZ] > c[renVARYW]) c_clip = 1;

  //printf("[%f, %f, %f, %f]\n",a[0],a[1],a[2],a[3] );

  if (a_clip == 1 && b_clip == 1 && c_clip == 1) {
    //printf("clipped triangle\n");
    return;
  } else if (a_clip == 0 && b_clip == 0 && c_clip == 0) {

    double view_a[renVARYDIMBOUND], view_b[renVARYDIMBOUND], view_c[renVARYDIMBOUND];

    //doViewPort(renRenderer *ren, double ogvert[] , double transVert[])
    doViewPort(ren,a,view_a);
    doViewPort(ren,b,view_b);
    doViewPort(ren,c,view_c);

    //printf("rendering... [%f,%f,%f] [%f,%f,%f] [%f,%f,%f]\n",a[0],a[1],a[2],b[0],b[1],b[2],c[0],c[1],c[2]);
    triRender(ren, unif, tex, view_a, view_b, view_c);
  } else {
    if (a_clip == 1 && b_clip == 0 && c_clip == 0) {
      //printf("1\n");
      clip_one(ren, unif, tex, a, b, c, 1);
    } else if (a_clip == 1 && b_clip == 1 && c_clip == 0) {
      //printf("2\n");
      clip_two(ren, unif, tex, a, b, c, 1, 2);
    } else if (a_clip == 1 && b_clip == 0 && c_clip == 1) {
      //printf("3\n");
      clip_two(ren, unif, tex, a, b, c, 1, 3);
    } else if (a_clip == 0 && b_clip == 1 && c_clip == 0) {
      //printf("4\n");
      clip_one(ren, unif, tex, a, b, c, 2);
    } else if (a_clip == 0 && b_clip == 1 && c_clip == 1) {
      //printf("5\n");
      clip_two(ren, unif, tex, a, b, c, 2, 3);
    } else if (a_clip == 0 && b_clip == 0 && c_clip == 1) {
      //printf("6\n");
      clip_one(ren, unif, tex, a, b, c, 3);
    }
  }
}
