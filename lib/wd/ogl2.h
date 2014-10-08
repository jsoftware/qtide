#ifndef OGL2_H
#define OGL2_H

extern "C" {
  Dllexport int gl_arc (const int *p);
  Dllexport int gl_brush ();
  Dllexport int gl_brushnull ();
  Dllexport int gl_capture (int a);
  Dllexport int gl_caret (const int *p);
  Dllexport int gl_clear ();
  Dllexport int gl_clear2 (void *,int);
  Dllexport int gl_clip (const int *);
  Dllexport int gl_clipreset ();
  Dllexport int gl_cmds (const int *, const int);
  Dllexport int gl_cursor (int a);
  Dllexport int gl_ellipse (const int *p);
  Dllexport int gl_fill (const int *p);
  Dllexport int gl_font (char *face);
  Dllexport int gl_font2 (const int *p, int len);
  Dllexport int gl_fontangle (int a);
  Dllexport int gl_fontextent (char *face);
  Dllexport int gl_lines (const int *p, int len);
  Dllexport int gl_nodblbuf (int a);
  Dllexport int gl_paint ();
  Dllexport int gl_paintx ();
  Dllexport int gl_pen (const int *p);
  Dllexport int gl_pie (const int *p);
  Dllexport int gl_pixel (const int *p);
  Dllexport int gl_pixels (const int *, int);
  Dllexport int gl_pixelsx (const int *p);
  Dllexport int gl_polygon (const int *p, int len);
  Dllexport int gl_qextent (char *s,int *wh);
  Dllexport int gl_qextentw (char *s,int *w);
  Dllexport int gl_qhandles (void **p);
  Dllexport int gl_qpixels (const int *p,int *pix);
  Dllexport int gl_qpixelm (const int *p,int *shape,int *pix);
  Dllexport int gl_qtextmetrics (int *tm);
  Dllexport int gl_qwh (int *wh);
  Dllexport int gl_rect (const int *p);
  Dllexport int gl_rgb (const int *p);
  Dllexport int gl_rgba (const int *p);
  Dllexport int gl_sel (void *g);
  Dllexport int gl_sel2 (char *id);
  Dllexport int gl_setlocale (char *loc);
  Dllexport int gl_text (char *ys);
  Dllexport int gl_textcolor ();
  Dllexport int gl_textxy (const int *p);
  Dllexport int gl_windoworg (const int *p);

  Dllexport int gl_updategl(void *g);
}

#endif
