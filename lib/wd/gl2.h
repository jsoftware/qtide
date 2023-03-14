#ifndef GL2_H
#define GL2_H

#include "../base/base.h"

#define glarc_n   2001
#define glbrush_n   2004
#define glbrushnull_n   2005
#define glcapture_n   2062
#define glcaret_n   2065
#define glclear_n   2007
#define glclip_n   2078
#define glclipreset_n   2079
#define glcmds_n   2999
#define glcursor_n   2069
#define glellipse_n   2008
#define glemfopen_n   2084
#define glemfclose_n   2085
#define glemfplay_n   2086
#define glfile_n   2066
#define glfill_n   2093
#define glfont_n   2012
#define glfontextent_n   2094
#define gllines_n   2015
#define glnodblbuf_n   2070
#define glpaint_n   2020
#define glpaintx_n   2021
#define glpen_n   2022
#define glpie_n   2023
#define glpixel_n   2024
#define glpixelsx_n   2075
#define glpixels_n   2076
#define glprint_n   2089
#define glprintmore_n   2091
#define glpolygon_n   2029
#define glqextent_n   2057
#define glqextentw_n   2083
#define glqpixels_n   2077
#define glqpixelm_n   2080
#define glqprintpaper_n   2092
#define glqprintwh_n   2088
#define glqtextmetrics_n   2058
#define glqtype_n   2095
#define glqwh_n   2059
#define glqhandles_n   2060
#define glrect_n   2031
#define glrgb_n   2032
#define glroundr_n   2033
#define glsel_n   2035
#define gltext_n   2038
#define gltextcolor_n   2040
#define gltextxy_n   2056
#define glwindoworg_n   2045

#define glbkmode_n   2003
#define glnoerasebkgnd_n   2071
#define glsetlocale_n   2072

#define glfont2_n   2312      // hardwired in plot addon
#define glfontangle_n   2096
#define glrgba_n   2097
#define glsel2_n   2098

// image utility
#define glgetimg_n   3000
#define glreadimg_n   3001
#define glputimg_n   3002
#define glwriteimg_n   3003

extern "C" {
  Dllexport int glarc (const int *p);
  Dllexport int glbrush ();
  Dllexport int glbrushnull ();
  Dllexport int glcapture (int a);
  Dllexport int glcaret (const int *p);
  Dllexport int glclear ();
  Dllexport int glclear2 (void *,int);
  Dllexport int glclip (const int *);
  Dllexport int glclipreset ();
  Dllexport int glcmds (const int *, const int);
  Dllexport int glcursor (int a);
  Dllexport int glellipse (const int *p);
  Dllexport int glfill (const int *p);
  Dllexport int glfont (char *face);
  Dllexport int glfont2 (const int *p, int len);
  Dllexport int glfontangle (int a);
  Dllexport int glfontextent (char *face);
  Dllexport int gllines (const int *p, int len);
  Dllexport int glnodblbuf (int a);
  Dllexport int glpaint ();
  Dllexport int glpaintx ();
  Dllexport int glpen (const int *p);
  Dllexport int glpie (const int *p);
  Dllexport int glpixel (const int *p);
  Dllexport int glpixels (const int *, int);
  Dllexport int glpixelsx (const int *p);
  Dllexport int glpolygon (const int *p, int len);
  Dllexport int glqextent (char *s,int *wh);
  Dllexport int glqextentw (char *s,int *w);
  Dllexport int glqhandles (void **p);
  Dllexport int glqpixels (const int *p,int *pix);
  Dllexport int glqpixelm (const int *p,int *shape,int *pix);
  Dllexport int glqtextmetrics (int *tm);
  Dllexport int glqwh (int *wh);
  Dllexport int glrect (const int *p);
  Dllexport int glrgb (const int *p);
  Dllexport int glrgba (const int *p);
  Dllexport int glsel (void *g);
  Dllexport int glsel2 (char *id);
  Dllexport int glsetlocale (char *loc);
  Dllexport int gltext (char *ys);
  Dllexport int gltextcolor ();
  Dllexport int gltextxy (const int *p);
  Dllexport int glqtype(int *type);
  Dllexport int glwindoworg (const int *p);

  Dllexport int glquery (int t, int *ptr, int ncnt);
  Dllexport int glquery2 ( int t, int *ptr, int ncnt);
}

#endif
