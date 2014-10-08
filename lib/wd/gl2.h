#ifndef GL2_H
#define GL2_H

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
  Dllexport int glwindoworg (const int *p);

}

#endif
