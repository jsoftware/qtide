#ifndef GLZ_H
#define GLZ_H

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
  Dllexport int glzsel (void *g);
  Dllexport int glzsel2 (char *id);
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
}

#endif
