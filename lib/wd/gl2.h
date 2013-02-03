#ifndef GL2_H
#define GL2_H

extern "C" {
  int Dllexport glarc (const int *p);
  int Dllexport glbrush ();
  int Dllexport glbrushnull ();
  int Dllexport glcapture (int a);
  int Dllexport glcaret (const int *p);
  int Dllexport glclear ();
  int Dllexport glclip (const int *);
  int Dllexport glclipreset ();
  int Dllexport glcmds (const int *, const int);
  int Dllexport glcursor (int a);
  int Dllexport glellipse (const int *p);
  int Dllexport glfont (char *face);
  int Dllexport glfont2 (const int *p, int len);
  int Dllexport glfontangle (int a);
  int Dllexport gllines (const int *p, int len);
  int Dllexport glnodblbuf (int a);
  int Dllexport glpaint ();
  int Dllexport glpaintx ();
  int Dllexport glpen (const int *p);
  int Dllexport glpie (const int *p);
  int Dllexport glpixel (const int *p);
  int Dllexport glpixels (const int *, int);
  int Dllexport glpixelsx (const int *p);
  int Dllexport glpolygon (const int *p, int len);
  int Dllexport glqextent (char *s,int *wh);
  int Dllexport glqextentw (char *s,int *w);
  int Dllexport glqhandles (void **p);
  int Dllexport glqpixels (const int *p,int *pix);
  int Dllexport glqtextmetrics (int *tm);
  int Dllexport glqwh (int *wh);
  int Dllexport glrect (const int *p);
  int Dllexport glrgb (const int *p);
  int Dllexport glsel (void *g);
  int Dllexport glsel2 (char *id);
  int Dllexport gltext (char *ys);
  int Dllexport gltextcolor ();
  int Dllexport gltextxy (const int *p);
  int Dllexport glwindoworg (const int *p);

  int Dllexport glclear2 (void *);
  int Dllexport glsetlocale (char *loc);
}

#endif
