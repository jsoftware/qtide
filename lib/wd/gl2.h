#ifndef GL2_H
#define GL2_H

extern "C" {
  int glarc (const int *p);
  int glbrush ();
  int glbrushnull ();
  int glcapture (int a);
  int glcaret (const int *p);
  int glclear ();
  int glclip (const int *);
  int glclipreset ();
  int glcmds (const int *, const int);
  int glcursor (int a);
  int glellipse (const int *p);
  int glfont (char *face);
  int glfont2 (const int *p, int len);
  int glfontangle (int a);
  int gllines (const int *p, int len);
  int glnodblbuf (int a);
  int glpaint ();
  int glpaintx ();
  int glpen (const int *p);
  int glpie (const int *p);
  int glpixel (const int *p);
  int glpixels (const int *, int);
  int glpixelsx (const int *p);
  int glpolygon (const int *p, int len);
  int glqextent (char* s,int *wh);
  int glqextentw (char* s,int *w);
  int glqhandles (void**p);
  int glqpixels (const int *p,int *pix);
  int glqtextmetrics (int *tm);
  int glqwh (int *wh);
  int glrect (const int *p);
  int glrgb (const int *p);
  int glsel (void *g);
  int glsel2 (char *id);
  int gltext (char *ys);
  int gltextcolor ();
  int gltextxy (const int *p);
  int glwindoworg (const int *p);

  int glclear2 (void *);
  int glsetlocale (char *loc);
}

#endif
