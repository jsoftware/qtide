#ifndef GLZ_H
#define GLZ_H

#include "../base/base.h"

#define glzarc_n   2201
#define glzbrush_n   2202
#define glzbrushnull_n   2203
#define glzclear_n   2204
#define glzclip_n   2205
#define glzclipreset_n   2206
#define glzcmds_n   2207
#define glzellipse_n   2208
#define glzfill_n   2209
#define glzfont_n   2210
#define glzfont2_n   2211
#define glzfontangle_n   2212
#define glzfontextent_n   2213
#define glzlines_n   2214
#define glznodblbuf_n   2215
#define glzpen_n   2216
#define glzpie_n   2217
#define glzpixel_n   2218
#define glzpixels_n   2219
#define glzpixelsx_n   2220
#define glzpolygon_n   2221
#define glzrect_n   2222
#define glzrgb_n   2223
#define glzrgba_n   2224
#define glztext_n   2225
#define glztextcolor_n   2226
#define glztextxy_n   2227
#define glzwindoworg_n   2228
#define glzresolution_n   2229
#define glzcolormode_n   2230
#define glzduplexmode_n   2231
#define glzorientation_n   2232
#define glzoutputformat_n   2233
#define glzpageorder_n   2234
#define glzpapersize_n   2235
#define glzpapersource_n   2236
#define glzscale_n   2237
#define glzabortdoc_n   2238
#define glzenddoc_n   2239
#define glznewpage_n   2240
#define glzprinter_n   2241
#define glzstartdoc_n   2242
#define glzinitprinter_n   2243
#define glzqresolution_n   2244
#define glzqcolormode_n   2245
#define glzqduplexmode_n   2246
#define glzqorientation_n   2247
#define glzqoutputformat_n   2248
#define glzqpageorder_n   2249
#define glzqpapersize_n   2250
#define glzqpapersource_n   2251
#define glzqwh_n   2252
#define glzqmargins_n   2253
#define glzqextent_n   2254
#define glzqextentw_n   2255
#define glzqtextmetrics_n   2256
#define glzcapture_n   2257
#define glzcaret_n   2258
#define glzcursor_n   2259
#define glzqtype_n   2260
#define glzpaint_n    2261
#define glzpaintx_n   2262
#define glzsetlocale_n   2263

extern "C" {
  Dllexport int glzarc (const int *p);
  Dllexport int glzbrush ();
  Dllexport int glzcapture (int a);
  Dllexport int glzcaret (const int *p);
  Dllexport int glzbrushnull ();
  Dllexport int glzclear ();
  Dllexport int glzclear2 (void *,int);
  Dllexport int glzclip (const int *);
  Dllexport int glzclipreset ();
  Dllexport int glzcmds (const int *, const int);
  Dllexport int glzcursor (int a);
  Dllexport int glzellipse (const int *p);
  Dllexport int glzfill (const int *p);
  Dllexport int glzfont (char *face);
  Dllexport int glzfont2 (const int *p, int len);
  Dllexport int glzfontangle (int a);
  Dllexport int glzfontextent (char *face);
  Dllexport int glzlines (const int *p, int len);
  Dllexport int glznodblbuf (int a);
  Dllexport int glzpaint ();
  Dllexport int glzpaintx ();
  Dllexport int glzpen (const int *p);
  Dllexport int glzpie (const int *p);
  Dllexport int glzpixel (const int *p);
  Dllexport int glzpixels (const int *, int);
  Dllexport int glzpixelsx (const int *p);
  Dllexport int glzpolygon (const int *p, int len);
  Dllexport int glzqextent (char *s,int *wh);
  Dllexport int glzqextentw (char *s,int *w);
  Dllexport int glzqhandles (void **p);
  Dllexport int glzqpixels (const int *p,int *pix);
  Dllexport int glzqpixelm (const int *p,int *shape,int *pix);
  Dllexport int glzqtextmetrics (int *tm);
  Dllexport int glzqwh (float *wh, int unit);
  Dllexport int glzrect (const int *p);
  Dllexport int glzrgb (const int *p);
  Dllexport int glzrgba (const int *p);
  Dllexport int glzsetlocale (char *loc);
  Dllexport int glztext (char *ys);
  Dllexport int glztextcolor ();
  Dllexport int glztextxy (const int *p);
  Dllexport int glzwindoworg (const int *p);

  Dllexport int glzqmargins(float *ltrb, int unit);
  Dllexport int glzscale (float *jobname);

  Dllexport int glzqresolution ();
  Dllexport int glzqcolormode ();
  Dllexport int glzqduplexmode ();
  Dllexport int glzqorientation ();
  Dllexport int glzqoutputformat ();
  Dllexport int glzqpageorder ();
  Dllexport int glzqpapersize ();
  Dllexport int glzqpapersource ();

  Dllexport int glzresolution (int n);
  Dllexport int glzcolormode (int n);
  Dllexport int glzduplexmode (int n);
  Dllexport int glzorientation (int n);
  Dllexport int glzoutputformat (int n);
  Dllexport int glzpageorder (int n);
  Dllexport int glzpapersize (int n);
  Dllexport int glzpapersource (int n);

  Dllexport int glzabortdoc ();
  Dllexport int glzenddoc ();
  Dllexport int glznewpage ();
  Dllexport int glzprinter (char *printername);
  Dllexport int glzstartdoc (char *jobname, char *filename);

  Dllexport int glzinitprinter ();

  Dllexport int glzquery (int t, int *ptr, int ncnt);
  Dllexport int glzquery2 ( int t, int *ptr, int ncnt);
}

#endif
