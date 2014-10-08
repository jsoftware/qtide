#ifndef GLC_H
#define GLC_H

extern "C" {
  Dllexport int glc_arc (const int *p);
  Dllexport int glc_brush ();
  Dllexport int glc_brushnull ();
  Dllexport int glc_capture (int a);
  Dllexport int glc_caret (const int *p);
  Dllexport int glc_clear ();
  Dllexport int glc_clear2 (void *,int);
  Dllexport int glc_clip (const int *);
  Dllexport int glc_clipreset ();
  Dllexport int glc_cmds (const int *, const int);
  Dllexport int glc_cursor (int a);
  Dllexport int glc_ellipse (const int *p);
  Dllexport int glc_fill (const int *p);
  Dllexport int glc_font (char *face);
  Dllexport int glc_font2 (const int *p, int len);
  Dllexport int glc_fontangle (int a);
  Dllexport int glc_fontextent (char *face);
  Dllexport int glc_lines (const int *p, int len);
  Dllexport int glc_nodblbuf (int a);
  Dllexport int glc_paint ();
  Dllexport int glc_paintx ();
  Dllexport int glc_pen (const int *p);
  Dllexport int glc_pie (const int *p);
  Dllexport int glc_pixel (const int *p);
  Dllexport int glc_pixels (const int *, int);
  Dllexport int glc_pixelsx (const int *p);
  Dllexport int glc_polygon (const int *p, int len);
  Dllexport int glc_qextent (char *s,int *wh);
  Dllexport int glc_qextentw (char *s,int *w);
  Dllexport int glc_qhandles (void **p);
  Dllexport int glc_qpixels (const int *p,int *pix);
  Dllexport int glc_qpixelm (const int *p,int *shape,int *pix);
  Dllexport int glc_qtextmetrics (int *tm);
  Dllexport int glc_qwh (int *wh);
  Dllexport int glc_rect (const int *p);
  Dllexport int glc_rgb (const int *p);
  Dllexport int glc_rgba (const int *p);
  Dllexport int glc_sel (void *g);
  Dllexport int glc_sel2 (char *id);
  Dllexport int glc_setlocale (char *loc);
  Dllexport int glc_text (char *ys);
  Dllexport int glc_textcolor ();
  Dllexport int glc_textxy (const int *p);
  Dllexport int glc_windoworg (const int *p);

  Dllexport int glc_free ();
  Dllexport int glc_init (const int *p);
  Dllexport int glc_savefile (char *f);

}

#endif
